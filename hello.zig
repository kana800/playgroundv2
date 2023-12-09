const std = @import("std");

pub fun main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Heelo world");
}
