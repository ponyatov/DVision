module nafigator;

import std.stdio;
import std.range;

import dv;

class MyApp : VApplication {
}

MyApp app;

void main(string[] args) {
    foreach (argc, argv; args.enumerate)
        arg(argc, argv);
    app.init.run.done;
}

void arg(ulong argc, string argv) {
    writefln("argv[%s] = <%s>", argc, argv);
}
