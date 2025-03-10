// Iterative version
// pub fn find<U, T>(array: U, key: T) -> Option<usize>
//     where
//         U: AsRef<[T]>,
//         T: PartialEq + PartialOrd,
// {
//     let array = array.as_ref();
//
//     if array.len() == 0 {
//         return None;
//     }
//
//     if key < *array.first().unwrap() || key > *array.last().unwrap() {
//         return None;
//     }
//
//     let mut cur = array;
//     let mut idx: usize = 0;
//     loop {
//         let (left, right) = cur.split_at(cur.len() / 2);
//         if right.len() == 0 {
//             break;
//         }
//         if key == right[0] {
//             return Some(idx + left.len());
//         }
//         if key < right[0] {
//             cur = left;
//         } else {
//             cur = &right[1..];
//             idx += left.len() + 1;
//         }
//     }
//
//     return None;
// }

// Recursive version
pub fn find<U, T>(array: U, key: T) -> Option<usize>
    where
        U: AsRef<[T]>,
        T: PartialEq + PartialOrd,
{
    let array = array.as_ref();

    if array.len() == 0 {
        return None;
    }

    if key < *array.first().unwrap() || key > *array.last().unwrap() {
        return None;
    }

    let mid = array.len() / 2;
    if array[mid] == key {
        return Some(mid);
    }

    return if array[mid] > key {
        match find(&array[..mid], key) {
            Some(idx) => Some(idx),
            None => None,
        }
    } else {
        match find(&array[mid..], key) {
            Some(idx) => Some(mid + idx),
            None => None,
        }
    };
}
