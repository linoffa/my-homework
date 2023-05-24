import subprocess as sb
import time
import sys
def run(args):
    return sb.run(args,
        capture_output=True, ).stdout.decode().strip()

def main():
    if (len(sys.argv) < 6):
        print("Invalid arguments")
        return
    exe_path = sys.argv[1]
    out_path = sys.argv[2]
    right_bound = int(sys.argv[3])
    step = int(sys.argv[4])
    iter = sys.argv[5]

    f = open(out_path, "w")
    f.write(f"{exe_path}\n")
    f.close()
    for expr_len in range(1, right_bound, step):
        test_string = "+".join(['2'] * expr_len)
        args = [exe_path, test_string, iter]
        t = time.monotonic()
        run(args)
        end_t = time.monotonic()
        f = open(out_path, "a")
        f.write(f"{expr_len} {(end_t - t) / (int(iter))}\n")
        f.close()
        print(f" Step {expr_len} finished")
        
if __name__ == "__main__":
    main()