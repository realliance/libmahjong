use cmake;

fn main() {
    let dst = cmake::Config::new("..").build_target("libmahjong").build();

    println!("cargo:rustc-link-search=native={}/build", dst.display());
    println!("cargo:rustc-link-lib=dylib=mahjong");
}
