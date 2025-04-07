import matplotlib.pyplot as plt
import numpy as np

def readData(filename):
    file = open(filename, "r")

    data = [x for x in map(lambda x: x.split(), file.readlines())]

    x_coords = []
    y_coords = []

    for i in range(100):
        x_coords.append(int(data[i][0]))
        y_coords.append(float(data[i][1]))

    file.close()

    return [x_coords, y_coords]

def drawPlotDiffOpt(no_op, arm, op_100_4):
    colors = plt.cm.rainbow([i/6 for i in range(6)])
    plt.scatter(no_op[0],    no_op[1],    s=1, color=colors[0], label="No optimization")
    plt.scatter(arm[0],      arm[1],      s=1, color=colors[1], label="ARM Neon intrinsics")
    plt.scatter(op_100_4[0], op_100_4[1], s=1, color=colors[2], label="x4 Unrolling")

    plt.yticks(np.arange(0, 12, 1))
    plt.title("Time plot of FPS with different optimizations")
    plt.ylabel("FPS")
    plt.xlabel("Iteration index")
    plt.legend()

    plt.savefig("imgs/diff_opt.png")

    return

def drawPlotVersus(op_100_16, op_100_32):
    plt.clf()
    colors = plt.cm.rainbow([i/6 for i in range(6)])
    plt.scatter(op_100_16[0], op_100_16[1], s=1, color=colors[0], label="x16 Unrolling")
    plt.scatter(op_100_32[0], op_100_32[1], s=1, color=colors[1], label="x32 Unrolling")

    plt.yticks(np.arange(0, 27, 1))
    plt.title("Time plot of FPS with x32 and x16 unrolling")
    plt.ylabel("FPS")
    plt.xlabel("Iteration index")
    plt.legend()

    plt.savefig("imgs/16_vs_32.png")

    return

def drawPlotDiffCoefs(op_100_4, op_100_8, op_100_12, op_100_16):
    colors = plt.cm.rainbow([i/6 for i in range(6)])
    plt.scatter(op_100_8[0],  op_100_8[1],  s=1, color=colors[3], label="x8 Unrolling")
    plt.scatter(op_100_12[0], op_100_12[1], s=1, color=colors[4], label="x12 Unrolling")
    plt.scatter(op_100_16[0], op_100_16[1], s=1, color=colors[5], label="x16 Unrolling")

    plt.yticks(np.arange(0, 27, 1))
    plt.title("Time plot of FPS with different loop unrolling coefficients")
    plt.ylabel("FPS")
    plt.xlabel("Iteration index")
    plt.legend()

    plt.savefig("imgs/diff_coefs.png")

    return

def main():
    no_op    = readData("../data/no_op_100.txt")
    arm      = readData("../data/arm_100.txt")
    op_100_4 = readData("../data/op_100_4.txt")

    drawPlotDiffOpt(no_op, arm, op_100_4)

    op_100_8  = readData("../data/op_100_8.txt")
    op_100_12 = readData("../data/op_100_12.txt")
    op_100_16 = readData("../data/op_100_16.txt")

    drawPlotDiffCoefs(op_100_4, op_100_8, op_100_12, op_100_16)

    op_100_32 = readData("../data/op_100_32.txt")

    drawPlotVersus(op_100_16, op_100_32)

    return

main()
