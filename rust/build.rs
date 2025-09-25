use std::path::PathBuf;

fn main() {
    let dst = cmake::Config::new("..")
        .build_target("libmahjong")
        .define("libmahjong_build_tests", "OFF")
        .define("libmahjong_build_tools", "OFF")
        .define("libmahjong_use_clang_utils", "OFF")
        .define("BUILD_SHARED_LIBS", "ON")
        .build();

    println!("cargo:rustc-link-search=native={}/build", dst.display());
    println!("cargo:rustc-link-lib=dylib=mahjong");

    let cpp_src_dir = PathBuf::from("..").join("src");
    let rust_include_dir = PathBuf::from("include");

    // All bridge files
    let bridge_files = vec!["src/types/wind.rs", "src/types/piece.rs"];

    let mut bridge_build = cxx_build::bridges(bridge_files.clone());
    bridge_build
        .include(&cpp_src_dir)
        .include(&rust_include_dir)
        .std("c++23")
        .compile("libmahjong-bridge");

    for file in bridge_files {
        println!("cargo:rerun-if-changed={}", file);
    }

    println!("cargo:rerun-if-changed=../src");
}
