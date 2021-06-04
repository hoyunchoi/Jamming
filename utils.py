import argparse
import typing
import pandas as pd
import numpy as np

def set_default() -> argparse.Namespace:
    args = argparse.Namespace()

    #* Network parameters
    args.network_size = 10000
    args.link_size = 25000
    args.degree_exponent = 2.2
    args.network_seed = 0

    #* Dynamics parameters
    args.strategy = 0.85
    args.packet = 10
    args.max_iteration = 10000
    args.dynamics_seed = 0

    #* Observation parameters
    args.steady = 1000
    args.time_window = 10

    return args


def network_prefix(param: argparse.Namespace) -> str:
    prefix = 'N{}L{}G{:.1f}'.format(param.network_size,
                                    param.link_size,
                                    param.degree_exponent)

    if param.network_seed == -1:
        return prefix
    else:
        return prefix + '-{}'.format(param.network_seed)


def dynamics_prefix(param: argparse.Namespace) -> str:
    prefix = 'S{:.2f}P{}I{}'.format(param.strategy,
                                    param.packet,
                                    param.max_iteration)
    if param.network_seed == -1:
        return prefix
    else:
        return prefix + '-{}'.format(param.network_seed)


def dynamics_prefix_wo_packet(param: argparse.Namespace) -> str:
    prefix = 'S{:.2f}I{}'.format(param.strategy,
                                 param.max_iteration)
    if param.network_seed == -1:
        return prefix
    else:
        return prefix + '-{}'.format(param.network_seed)


def history2op(raw: np.ndarray, param: argparse.Namespace) -> typing.Tuple[np.float64, np.float64]:
    op = np.zeros(len(raw) - param.steady - param.time_window)
    for i, index in enumerate(range(param.steady, len(raw) - param.time_window)):
        op[i] = max(0.0, raw[index + param.time_window] - raw[index])
    op /= param.time_window * param.packet
    mean = np.mean(op)
    sem = np.std(op, ddof=1) / np.sqrt(len(op))
    return mean, sem

if __name__ == "__main__":
    print("This is module read_data")
