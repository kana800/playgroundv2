const std = @import("std");
const print = std.debug.print;

pub fn main() void {
    const t1 = [4]u8{10,15,3,7};
    for (t1) |i| {
        for (t1) |j| {
            if ((i+j) == 17)
            {
                print("{} + {} = 17\n", .{i, j});
                break;
            }
        }
    }
}
