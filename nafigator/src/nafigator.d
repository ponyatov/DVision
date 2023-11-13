module nafigator;

import std.stdio;
import std.range;

void main(string[] args) {
    foreach (argc, argv; args.enumerate)
        arg(argc, argv);
}

void arg(ulong argc, string argv) {
    writefln("argv[%s] = <%s>", argc, argv);
}
