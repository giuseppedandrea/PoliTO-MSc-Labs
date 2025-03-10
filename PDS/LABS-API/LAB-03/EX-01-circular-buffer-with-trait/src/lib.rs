pub struct CircularBuffer<T> {
    buffer: Vec<T>,
    len: usize,
    capacity: usize,
    write_index: usize,
    read_index: usize,
}

#[derive(Debug, PartialEq)]
pub enum Error {
    EmptyBuffer,
    FullBuffer,
}

impl<T> CircularBuffer<T> where T: Default {
    pub fn new(capacity: usize) -> Self {
        Self {
            buffer: std::iter::repeat_with(|| T::default()).take(capacity).collect(),
            len: 0,
            capacity,
            write_index: 0,
            read_index: 0,
        }
    }

    pub fn write(&mut self, _element: T) -> Result<(), Error> {
        if self.is_full() {
            return Err(Error::FullBuffer);
        }
        self.buffer[self.write_index] = _element;
        self.advance_write_index();
        self.len += 1;
        Ok(())
    }

    pub fn read(&mut self) -> Result<T, Error> {
        if self.is_empty() {
            return Err(Error::EmptyBuffer);
        }
        let value = self.replace_value(self.read_index, T::default());
        self.advance_read_index();
        self.len -= 1;
        Ok(value)
    }

    pub fn clear(&mut self) {
        for idx in 0..self.capacity {
            self.replace_value(idx, T::default());
        }
        self.read_index = 0;
        self.write_index = 0;
        self.len = 0;
    }

    pub fn overwrite(&mut self, _element: T) {
        self.replace_value(self.write_index, _element);
        self.advance_write_index();
        if self.is_full() {
            self.advance_read_index();
        } else {
            self.len += 1;
        }
    }

    fn is_full(&self) -> bool {
        self.len >= self.capacity
    }

    fn is_empty(&self) -> bool {
        self.len == 0
    }

    fn replace_value(&mut self, index: usize, value: T) -> T {
        // Move `src` into the referenced `dest`, returning the previous `dest` value
        std::mem::replace(&mut self.buffer[index], value)
    }

    fn advance_write_index(&mut self) {
        self.write_index = (self.write_index + 1) % self.capacity;
    }

    fn advance_read_index(&mut self) {
        self.read_index = (self.read_index + 1) % self.capacity;
    }
}
