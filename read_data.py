import argparse


def set_default() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    args = parser.parse_args('')

    #* Network parameters
    args.network_size = 10000
    args.link_size = 200000
    args.degree_exponent = 2.2
    args.network_seed = 0

    #* Dynamics parameters
    args.strategy = 1.0
    args.new_packet = 8
    args.max_iteration = 200
    args.dynamics_seed = 0

    return args


def network_name(param: argparse.Namespace) -> str:
    name = 'N{}L{}G{:.1f}-{}'.format(param.network_size,
                                     param.link_size,
                                     param.degree_exponent,
                                     param.network_seed)

    return name


def dynamics_name(param: argparse.Namespace) -> str:
    name = 'S{:.2f}P{}I{}-{}'.format(param.strategy,
                                     param.new_packet,
                                     param.max_iteration,
                                     param.dynamics_seed)
    return name
