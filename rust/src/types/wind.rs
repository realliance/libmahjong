use std::fmt;

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Wind {
    East = 0,
    South = 1,
    West = 2,
    North = 3,
}

unsafe impl cxx::ExternType for Wind {
    type Id = cxx::type_id!("mahjong::Wind");
    type Kind = cxx::kind::Trivial;
}

#[cxx::bridge(namespace = "mahjong")]
mod ffi {

    unsafe extern "C++" {
        include!("types/winds.h");

        type Wind = super::Wind;
    }
}

impl fmt::Display for Wind {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let s = match *self {
            Wind::East => "East",
            Wind::South => "South",
            Wind::West => "West",
            Wind::North => "North",
        };
        write!(f, "{}", s)
    }
}

impl Wind {
    pub fn is_east(&self) -> bool {
        matches!(*self, Wind::East)
    }

    pub fn is_south(&self) -> bool {
        matches!(*self, Wind::South)
    }

    pub fn is_west(&self) -> bool {
        matches!(*self, Wind::West)
    }

    pub fn is_north(&self) -> bool {
        matches!(*self, Wind::North)
    }

    /// Get the next wind in clockwise order
    pub fn next(&self) -> Wind {
        match *self {
            Wind::East => Wind::South,
            Wind::South => Wind::West,
            Wind::West => Wind::North,
            Wind::North => Wind::East,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn wind_enum() {
        let east = Wind::East;

        assert_eq!(east.to_string(), "East");
        assert!(east.is_east());
        assert!(!east.is_south());
        assert_eq!(east.next(), Wind::South);
        assert_eq!(Wind::North.next(), east);
    }
}
