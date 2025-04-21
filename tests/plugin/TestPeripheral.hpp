// RUN: clang++ -fsyntax-only -Xclang -load -Xclang %path/to/libEOSPlugin.so -Xclang -plugin=eos-plugin -I../../include %s
// CHECK: class TestPeripheralShared

peripheral TestPeripheral {
    SharedVar int v;
    Service void f();
};
