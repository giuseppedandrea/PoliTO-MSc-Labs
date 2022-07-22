use std::collections::{HashMap, HashSet};

/// `InputCellId` is a unique identifier for an input cell.
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct InputCellId(usize);

/// `ComputeCellId` is a unique identifier for a compute cell.
/// Values of type `InputCellId` and `ComputeCellId` should not be mutually assignable,
/// demonstrated by the following tests:
///
/// ```compile_fail
/// let mut r = react::Reactor::new();
/// let input: react::ComputeCellId = r.create_input(111);
/// ```
///
/// ```compile_fail
/// let mut r = react::Reactor::new();
/// let input = r.create_input(111);
/// let compute: react::InputCellId = r.create_compute(&[react::CellId::Input(input)], |_| 222).unwrap();
/// ```
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct ComputeCellId(usize);

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct CallbackId(usize);

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub enum CellId {
    Input(InputCellId),
    Compute(ComputeCellId),
}

#[derive(Debug, PartialEq)]
pub enum RemoveCallbackError {
    NonexistentCell,
    NonexistentCallback,
}

struct ComputeCell<'a, T> {
    val: Option<T>,
    deps: Vec<CellId>,
    fun: Box<dyn Fn(&[T]) -> T + 'a>,
    cbs: HashMap<CallbackId, Box<dyn FnMut(T) + 'a>>,
}

pub struct Reactor<'a, T> {
    inputs: Vec<T>,
    computes: Vec<ComputeCell<'a, T>>,
    inv_deps: HashMap<CellId, HashSet<ComputeCellId>>,
    callbacks: HashMap<CallbackId, ComputeCellId>,
    callbacks_count: usize,
}

// You are guaranteed that Reactor will only be tested against types that are Copy + PartialEq.
impl<'a, T: Copy + PartialEq> Reactor<'a, T> {
    pub fn new() -> Self {
        Reactor {
            inputs: vec![],
            computes: vec![],
            inv_deps: HashMap::new(),
            callbacks: HashMap::new(),
            callbacks_count: 0
        }
    }

    // Creates an input cell with the specified initial value, returning its ID.
    pub fn create_input(&mut self, initial: T) -> InputCellId {
        let cell_id = InputCellId(self.inputs.len());
        self.inputs.push(initial);
        cell_id
    }

    // Creates a compute cell with the specified dependencies and compute function.
    // The compute function is expected to take in its arguments in the same order as specified in
    // `dependencies`.
    // You do not need to reject compute functions that expect more arguments than there are
    // dependencies (how would you check for this, anyway?).
    //
    // If any dependency doesn't exist, returns an Err with that nonexistent dependency.
    // (If multiple dependencies do not exist, exactly which one is returned is not defined and
    // will not be tested)
    //
    // Notice that there is no way to *remove* a cell.
    // This means that you may assume, without checking, that if the dependencies exist at creation
    // time they will continue to exist as long as the Reactor exists.
    pub fn create_compute<F: Fn(&[T]) -> T + 'a>(
        &mut self,
        dependencies: &[CellId],
        compute_func: F,
    ) -> Result<ComputeCellId, CellId> {
        for &dep in dependencies {
            match dep {
                CellId::Input(cell_id) => {
                    if cell_id.0 >= self.inputs.len() {
                        return Err(dep);
                    }
                }
                CellId::Compute(cell_id) => {
                    if cell_id.0 >= self.computes.len() {
                        return Err(dep);
                    }
                }
            }
        }

        let cell_id = ComputeCellId(self.computes.len());

        let compute_cell = ComputeCell {
            val: None,
            deps: dependencies.to_vec(),
            fun: Box::new(compute_func),
            cbs: HashMap::new()
        };
        self.computes.push(compute_cell);

        for &dep in dependencies {
            self.inv_deps.entry(dep).or_insert(HashSet::new()).insert(cell_id);
        }

        self.update_cell(&cell_id);

        Ok(cell_id)
    }

    // Retrieves the current value of the cell, or None if the cell does not exist.
    //
    // You may wonder whether it is possible to implement `get(&self, id: CellId) -> Option<&Cell>`
    // and have a `value(&self)` method on `Cell`.
    //
    // It turns out this introduces a significant amount of extra complexity to this exercise.
    // We chose not to cover this here, since this exercise is probably enough work as-is.
    pub fn value(&self, id: CellId) -> Option<T> {
        return match id {
            CellId::Input(cell_id) => {
                if cell_id.0 < self.inputs.len() {
                    Some(self.inputs[cell_id.0])
                } else {
                    None
                }
            }
            CellId::Compute(cell_id) => {
                if cell_id.0 < self.computes.len() {
                    self.computes[cell_id.0].val
                } else {
                    None
                }
            }
        };
    }

    // Sets the value of the specified input cell.
    //
    // Returns false if the cell does not exist.
    //
    // Similarly, you may wonder about `get_mut(&mut self, id: CellId) -> Option<&mut Cell>`, with
    // a `set_value(&mut self, new_value: T)` method on `Cell`.
    //
    // As before, that turned out to add too much extra complexity.
    pub fn set_value(&mut self, input_cell_id: InputCellId, new_value: T) -> bool {
        if input_cell_id.0 >= self.inputs.len() {
            return false;
        }
        self.inputs[input_cell_id.0] = new_value;

        let mut dirty: Vec<CellId> = vec![CellId::Input(input_cell_id)];

        while !dirty.is_empty() {
            let cur = dirty.pop().unwrap();

            let cur_deps = self.inv_deps.get(&cur).cloned();

            if let Some(deps) = cur_deps {
                for compute_cell_id in deps.iter() {
                    self.update_cell(&compute_cell_id);
                    dirty.push(CellId::Compute(*compute_cell_id))
                }
            }
        }

        true
    }

    // Adds a callback to the specified compute cell.
    //
    // Returns the ID of the just-added callback, or None if the cell doesn't exist.
    //
    // Callbacks on input cells will not be tested.
    //
    // The semantics of callbacks (as will be tested):
    // For a single set_value call, each compute cell's callbacks should each be called:
    // * Zero times if the compute cell's value did not change as a result of the set_value call.
    // * Exactly once if the compute cell's value changed as a result of the set_value call.
    //   The value passed to the callback should be the final value of the compute cell after the
    //   set_value call.
    pub fn add_callback<F: FnMut(T) + 'a>(
        &mut self,
        compute_cell_id: ComputeCellId,
        callback: F,
    ) -> Option<CallbackId> {
        if compute_cell_id.0 >= self.computes.len() {
            return None;
        }

        let callback_id = CallbackId(self.callbacks_count);
        self.callbacks_count += 1;

        let compute_cell = &mut self.computes[compute_cell_id.0];
        compute_cell.cbs.insert(callback_id, Box::new(callback));

        self.callbacks.insert(callback_id, compute_cell_id);

        Some(callback_id)
    }

    // Removes the specified callback, using an ID returned from add_callback.
    //
    // Returns an Err if either the cell or callback does not exist.
    //
    // A removed callback should no longer be called.
    pub fn remove_callback(
        &mut self,
        compute_cell_id: ComputeCellId,
        callback: CallbackId,
    ) -> Result<(), RemoveCallbackError> {
        if compute_cell_id.0 >= self.computes.len() {
            return Err(RemoveCallbackError::NonexistentCell);
        }
        match self.callbacks.get(&callback) {
            Some(_) => {
                self.computes[compute_cell_id.0].cbs.remove(&callback);
                self.callbacks.remove(&callback);
                Ok(())
            }
            None => Err(RemoveCallbackError::NonexistentCallback)
        }
    }

    fn update_cell(&mut self, compute_cell_id: &ComputeCellId) {
        let compute_cell = &self.computes[compute_cell_id.0];
        let args = compute_cell.deps.iter().map(|dep| {
            match dep {
                CellId::Input(cell_id) => { self.inputs[cell_id.0] }
                CellId::Compute(cell_id) => { self.computes[cell_id.0].val.unwrap() }
            }
        }).collect::<Vec<T>>();

        let compute_cell = &mut self.computes[compute_cell_id.0];
        let new_val = (*compute_cell.fun)(&args);
        if compute_cell.val.is_none() || compute_cell.val.unwrap().ne(&new_val) {
            compute_cell.val = Some(new_val);
            for (_, cb) in compute_cell.cbs.iter_mut() {
                (*cb)(new_val);
            }
        }
    }
}
