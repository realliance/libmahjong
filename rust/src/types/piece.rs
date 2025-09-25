use crate::types::{MahjongError, Wind};
use bitflags::{bitflags, bitflags_match};
use std::convert::TryFrom;
use std::fmt::{self, Display};
use std::ops::{Add, Sub};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Suit {
    Honor,
    Bamboo,
    Pin,
    Character,
}

impl Display for Suit {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let suit = match self {
            Suit::Honor => "z",
            Suit::Bamboo => "s",
            Suit::Pin => "p",
            Suit::Character => "m",
        };

        write!(f, "{suit}")
    }
}

#[derive(Debug, Clone, Copy)]
pub struct Piece(u8);

impl PartialEq for Piece {
    fn eq(&self, other: &Self) -> bool {
        (self.0 | Piece::RedFive.bits()) == (other.0 | Piece::RedFive.bits())
    }
}

impl Eq for Piece {}

bitflags! {
    impl Piece: u8 {
        const TerminalBit = 1 << 7;
        const HonorSuit = 0 << 5;
        const BambooSuit = 1 << 5;
        const PinSuit = 2 << 5;
        const CharacterSuit = 3 << 5;
        const RedFive = 1 << 4;
        const Error = 0;

        // Bamboo pieces
        const OneBamboo = Piece::BambooSuit.bits() | 1 | Piece::TerminalBit.bits();
        const TwoBamboo = Piece::BambooSuit.bits() | 2;
        const ThreeBamboo = Piece::BambooSuit.bits() | 3;
        const FourBamboo = Piece::BambooSuit.bits() | 4;
        const FiveBamboo = Piece::BambooSuit.bits() | 5;
        const RedFiveBamboo = Piece::BambooSuit.bits() | 5 | Piece::RedFive.bits();
        const SixBamboo = Piece::BambooSuit.bits() | 6;
        const SevenBamboo = Piece::BambooSuit.bits() | 7;
        const EightBamboo = Piece::BambooSuit.bits() | 8;
        const NineBamboo = Piece::BambooSuit.bits() | 9 | Piece::TerminalBit.bits();

        // Pin pieces
        const OnePin = Piece::PinSuit.bits() | 1 | Piece::TerminalBit.bits();
        const TwoPin = Piece::PinSuit.bits() | 2;
        const ThreePin = Piece::PinSuit.bits() | 3;
        const FourPin = Piece::PinSuit.bits() | 4;
        const FivePin = Piece::PinSuit.bits() | 5;
        const RedFivePin = Piece::PinSuit.bits() | 5 | Piece::RedFive.bits();
        const SixPin = Piece::PinSuit.bits() | 6;
        const SevenPin = Piece::PinSuit.bits() | 7;
        const EightPin = Piece::PinSuit.bits() | 8;
        const NinePin = Piece::PinSuit.bits() | 9 | Piece::TerminalBit.bits();

        // Man pieces
        const OneCharacter = Piece::CharacterSuit.bits() | 1 | Piece::TerminalBit.bits();
        const TwoCharacter = Piece::CharacterSuit.bits() | 2;
        const ThreeCharacter = Piece::CharacterSuit.bits() | 3;
        const FourCharacter = Piece::CharacterSuit.bits() | 4;
        const FiveCharacter = Piece::CharacterSuit.bits() | 5;
        const RedFiveCharacter = Piece::CharacterSuit.bits() | 5 | Piece::RedFive.bits();
        const SixCharacter = Piece::CharacterSuit.bits() | 6;
        const SevenCharacter = Piece::CharacterSuit.bits() | 7;
        const EightCharacter = Piece::CharacterSuit.bits() | 8;
        const NineCharacter = Piece::CharacterSuit.bits() | 9 | Piece::TerminalBit.bits();

        // Honor pieces
        const EastWind = Piece::HonorSuit.bits() | 1;
        const SouthWind = Piece::HonorSuit.bits() | 2;
        const WestWind = Piece::HonorSuit.bits() | 3;
        const NorthWind = Piece::HonorSuit.bits() | 4;
        const RedDragon = Piece::HonorSuit.bits() | 5;
        const WhiteDragon = Piece::HonorSuit.bits() | 6;
        const GreenDragon = Piece::HonorSuit.bits() | 7;
    }
}

unsafe impl cxx::ExternType for Piece {
    type Id = cxx::type_id!("mahjong::Piece");
    type Kind = cxx::kind::Trivial;
}

#[cxx::bridge(namespace = "mahjong")]
mod ffi {
    unsafe extern "C++" {
        include!("types/piecetype.h");
        include!("types/winds.h");
        include!("piece_adapter.h");

        type Piece = super::Piece;
        type Wind = crate::types::Wind;

        #[rust_name = "is_honor"]
        fn isHonor(self: &Piece) -> bool;
        #[rust_name = "is_terminal"]
        fn isTerminal(self: &Piece) -> bool;
        #[rust_name = "is_green"]
        fn isGreen(self: &Piece) -> bool;
        #[rust_name = "is_red_five"]
        fn isRedFive(self: &Piece) -> bool;
        #[rust_name = "is_board_piece"]
        fn isBoardPiece(self: &Piece) -> bool;
        #[rust_name = "as_u8"]
        fn toUint8_t(self: &Piece) -> u8;
        fn raw_value(self: &Piece) -> u8;
        #[rust_name = "get_suit_num"]
        fn getSuit(self: &Piece) -> u8;
        #[rust_name = "get_piece_num"]
        fn getPieceNum(self: &Piece) -> u8;

        #[rust_name = "from_wind"]
        #[Self = "Piece"]
        fn fromWind(w: Wind) -> Piece;

        #[rust_name = "form_piece_raw"]
        fn formPieceFromU8(suit: u8, number: u8, is_red_five: bool) -> Piece;
    }
}

impl fmt::Display for Piece {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        if let Some(suit) = self.suit() {
            let num = self.get_piece_num();
            let is_red = self.is_red_five();

            write!(f, "{}{}{}", if is_red { "r" } else { "" }, num, suit)
        } else {
            write!(f, "{}?", self.as_u8())
        }
    }
}

impl Piece {
    /// Returns a suit if it's a suited piece
    pub fn suit(&self) -> Option<Suit> {
        let suit_piece = Piece(self.get_suit_num());
        bitflags_match!(suit_piece, {
            Piece::HonorSuit => Some(Suit::Honor),
            Piece::BambooSuit => Some(Suit::Bamboo),
            Piece::PinSuit => Some(Suit::Pin),
            Piece::CharacterSuit => Some(Suit::Character),
            _ => None,
        })
    }

    /// Form a suited piece
    pub fn form_piece(suit: Suit, number: u8, is_red_five: bool) -> Result<Piece, MahjongError> {
        let suit_bits = match suit {
            Suit::Honor => Piece::HonorSuit.bits(),
            Suit::Bamboo => Piece::BambooSuit.bits(),
            Suit::Pin => Piece::PinSuit.bits(),
            Suit::Character => Piece::CharacterSuit.bits(),
        };

        let piece = ffi::form_piece_raw(suit_bits, number, is_red_five);

        if piece.raw_value() == 0 {
            Err(MahjongError::InvalidPiece { value: number })
        } else {
            Ok(piece)
        }
    }

    /// Convert to the wind a Wind piece is associated with it
    pub fn to_wind(&self) -> Option<Wind> {
        bitflags_match!(*self, {
            Piece::EastWind => Some(Wind::East),
            Piece::SouthWind => Some(Wind::South),
            Piece::WestWind => Some(Wind::West),
            Piece::NorthWind => Some(Wind::North),
            _ => None,
        })
    }

    /// Returns true if the piece is a Wind piece
    pub fn is_wind(&self) -> bool {
        *self == Piece::EastWind
            || *self == Piece::SouthWind
            || *self == Piece::WestWind
            || *self == Piece::NorthWind
    }

    /// Returns true if the piece is a Dragon Piece
    pub fn is_dragon(&self) -> bool {
        *self == Piece::RedDragon || *self == Piece::WhiteDragon || *self == Piece::GreenDragon
    }

    /// Returns true if the piece is a terminal or honor piece
    pub fn is_terminal_or_honor(&self) -> bool {
        self.is_terminal() || self.is_honor()
    }

    /// If the tile is a numbered piece, will return the next one in sequence
    ///
    /// If the piece is the final piece in the sequence, will return `None`
    pub fn next(&self) -> Option<Piece> {
        if self.is_honor() || self.get_piece_num() >= 9 {
            None
        } else {
            (*self + 1).ok()
        }
    }

    /// If the tile is a numbered piece, will return the previous one in sequence
    ///
    /// If the piece is the first piece in the sequence, will return `None`
    pub fn prev(&self) -> Option<Piece> {
        if self.is_honor() || self.get_piece_num() <= 1 {
            None
        } else {
            (*self - 1).ok()
        }
    }

    /// Add to piece number within the same suit, returning None if out of bounds
    pub fn add_within_suit(&self, n: u8) -> Option<Piece> {
        let suit = self.suit()?;
        if suit == Suit::Honor {
            return None;
        }

        let current_num = self.get_piece_num();
        let new_num = current_num.checked_add(n)?;
        if new_num > 9 {
            return None;
        }

        Piece::form_piece(suit, new_num, self.is_red_five()).ok()
    }

    /// Subtract from piece number within the same suit, returning None if out of bounds
    pub fn sub_within_suit(&self, n: u8) -> Option<Piece> {
        let suit = self.suit()?;
        if suit == Suit::Honor {
            return None;
        }

        let current_num = self.get_piece_num();
        let new_num = current_num.checked_sub(n)?;
        if new_num < 1 {
            return None;
        }

        Piece::form_piece(suit, new_num, self.is_red_five()).ok()
    }
}

impl Add<u8> for Piece {
    type Output = Result<Piece, MahjongError>;

    fn add(self, rhs: u8) -> Self::Output {
        self.add_within_suit(rhs).ok_or(MahjongError::InvalidPiece {
            value: self.as_u8().saturating_add(rhs),
        })
    }
}

impl Sub<u8> for Piece {
    type Output = Result<Piece, MahjongError>;

    fn sub(self, rhs: u8) -> Self::Output {
        self.sub_within_suit(rhs).ok_or(MahjongError::InvalidPiece {
            value: self.as_u8().saturating_sub(rhs),
        })
    }
}

impl TryFrom<u8> for Piece {
    type Error = MahjongError;

    fn try_from(value: u8) -> Result<Self, Self::Error> {
        Piece::from_bits(value).ok_or(MahjongError::InvalidPiece { value })
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn piece_display() {
        assert_eq!(format!("{}", Piece::OnePin), "1p");
        assert_eq!(format!("{}", Piece::RedFiveBamboo), "r5s");
        assert_eq!(format!("{}", Piece::EastWind), "1z");
        assert_eq!(format!("{}", Piece::RedDragon), "5z");
    }

    #[test]
    fn piece_properties() {
        let one_pin = Piece::OnePin;
        let east_wind = Piece::EastWind;
        let red_five = Piece::RedFiveBamboo;

        // Test honor detection
        assert!(!one_pin.is_honor());
        assert!(east_wind.is_honor());

        // Test terminal detection
        assert!(one_pin.is_terminal());
        assert!(!Piece::TwoPin.is_terminal());
        assert!(Piece::NinePin.is_terminal());

        // Test red five detection
        assert!(!one_pin.is_red_five());
        assert!(red_five.is_red_five());

        // Test suit detection
        assert_eq!(one_pin.suit(), Some(Suit::Pin));
        assert_eq!(east_wind.suit(), Some(Suit::Honor));
        assert_eq!(Piece::TwoBamboo.suit(), Some(Suit::Bamboo));
    }

    #[test]
    fn piece_equality() {
        assert_eq!(Piece::FivePin, Piece::FivePin);

        assert_eq!(Piece::FivePin, Piece::RedFivePin);
        assert_eq!(Piece::FiveBamboo, Piece::RedFiveBamboo);
    }

    #[test]
    fn piece_arithmetic() {
        let five_pin = Piece::FivePin;

        let six_pin = (five_pin + 1).expect("Should be able to add 1 to five pin");
        assert_eq!(six_pin.get_piece_num(), 6);
        assert_eq!(six_pin.suit(), Some(Suit::Pin));

        let four_pin = (five_pin - 1).expect("Should be able to subtract 1 from five pin");
        assert_eq!(four_pin.get_piece_num(), 4);
        assert_eq!(four_pin.suit(), Some(Suit::Pin));

        let nine_pin = Piece::NinePin;
        let error_piece = nine_pin + 1;
        assert!(error_piece.is_err());

        let one_pin = Piece::OnePin;
        let error_piece2 = one_pin - 1;
        assert!(error_piece2.is_err());
    }

    #[test]
    fn piece_from_wind() {
        let east_piece = Piece::from_wind(Wind::East);
        assert_eq!(east_piece, Piece::EastWind);
        assert!(east_piece.is_honor());

        let south_piece = Piece::from_wind(Wind::South);
        assert_eq!(south_piece, Piece::SouthWind);

        let west_piece = Piece::from_wind(Wind::West);
        assert_eq!(west_piece, Piece::WestWind);

        let north_piece = Piece::from_wind(Wind::North);
        assert_eq!(north_piece, Piece::NorthWind);
    }

    #[test]
    fn form_piece() {
        let five_pin = Piece::form_piece(Suit::Pin, 5, false).expect("Should create five pin");
        assert_eq!(five_pin, Piece::FivePin);
        assert_eq!(five_pin.get_piece_num(), 5);
        assert!(!five_pin.is_red_five());

        let red_five_pin =
            Piece::form_piece(Suit::Pin, 5, true).expect("Should create red five pin");
        assert_eq!(red_five_pin, Piece::RedFivePin);
        assert!(red_five_pin.is_red_five());

        let one_bamboo =
            Piece::form_piece(Suit::Bamboo, 1, false).expect("Should create one bamboo");
        assert_eq!(one_bamboo, Piece::OneBamboo);
        assert!(one_bamboo.is_terminal());

        let nine_char =
            Piece::form_piece(Suit::Character, 9, false).expect("Should create nine character");
        assert_eq!(nine_char, Piece::NineCharacter);
        assert!(nine_char.is_terminal());

        let east_wind = Piece::form_piece(Suit::Honor, 1, false).expect("Should create east wind");
        assert_eq!(east_wind, Piece::EastWind);
        assert!(east_wind.is_honor());

        let red_dragon =
            Piece::form_piece(Suit::Honor, 5, false).expect("Should create red dragon");
        assert_eq!(red_dragon, Piece::RedDragon);
        assert!(red_dragon.is_honor());

        let invalid = Piece::form_piece(Suit::Pin, 10, false); // Out of range
        assert!(invalid.is_err());

        let invalid_red_five = Piece::form_piece(Suit::Pin, 3, true); // Red five must be 5
        assert!(invalid_red_five.is_err());
    }

    #[test]
    fn piece_to_wind() {
        assert_eq!(Piece::EastWind.to_wind(), Some(Wind::East));
        assert_eq!(Piece::SouthWind.to_wind(), Some(Wind::South));
        assert_eq!(Piece::WestWind.to_wind(), Some(Wind::West));
        assert_eq!(Piece::NorthWind.to_wind(), Some(Wind::North));

        assert_eq!(Piece::RedDragon.to_wind(), None);
        assert_eq!(Piece::FivePin.to_wind(), None);
    }

    #[test]
    fn piece_is_wind() {
        assert!(Piece::EastWind.is_wind());
        assert!(Piece::SouthWind.is_wind());
        assert!(Piece::WestWind.is_wind());
        assert!(Piece::NorthWind.is_wind());

        assert!(!Piece::RedDragon.is_wind());
        assert!(!Piece::FivePin.is_wind());
    }

    #[test]
    fn piece_is_dragon() {
        assert!(Piece::RedDragon.is_dragon());
        assert!(Piece::WhiteDragon.is_dragon());
        assert!(Piece::GreenDragon.is_dragon());

        assert!(!Piece::EastWind.is_dragon());
        assert!(!Piece::FivePin.is_dragon());
    }

    #[test]
    fn sequence_operations() {
        let five_pin = Piece::FivePin;

        assert_eq!(five_pin.next(), Some(Piece::SixPin));
        assert_eq!(five_pin.prev(), Some(Piece::FourPin));

        assert_eq!(Piece::NinePin.next(), None);
        assert_eq!(Piece::OnePin.prev(), None);
        assert_eq!(Piece::EastWind.next(), None);
    }

    #[test]
    fn within_suit_arithmetic() {
        let five_pin = Piece::FivePin;

        assert_eq!(five_pin.add_within_suit(0), Some(five_pin));
        assert_eq!(five_pin.add_within_suit(1), Some(Piece::SixPin));
        assert_eq!(five_pin.add_within_suit(2), Some(Piece::SevenPin));
        assert_eq!(five_pin.add_within_suit(4), Some(Piece::NinePin));
        assert_eq!(five_pin.add_within_suit(5), None);

        assert_eq!(five_pin.sub_within_suit(0), Some(five_pin));
        assert_eq!(five_pin.sub_within_suit(1), Some(Piece::FourPin));
        assert_eq!(five_pin.sub_within_suit(4), Some(Piece::OnePin));
        assert_eq!(five_pin.sub_within_suit(5), None);

        let eight_bamboo = Piece::EightBamboo;
        let nine_bamboo = eight_bamboo.add_within_suit(1).unwrap();
        assert!(nine_bamboo.is_terminal());

        let two_bamboo = Piece::TwoBamboo;
        let one_bamboo = two_bamboo.sub_within_suit(1).unwrap();
        assert!(one_bamboo.is_terminal());

        assert_eq!(Piece::EastWind.add_within_suit(1), None);
        assert_eq!(Piece::RedDragon.sub_within_suit(1), None);
    }
}
