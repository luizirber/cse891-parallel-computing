from collections import defaultdict
infinite_defaultdict = lambda: defaultdict(infinite_defaultdict)

import pandas as pd


VECTOR_SIZE = [10, 100, 1000, 10000, 100000]
THREADS = [1, 2, 4, 8, 10, 16, 20]
#REPLICATES = range(1, 31)
REPLICATES = [1]


Panel5D = pd.core.panelnd.create_nd_panel_factory(
    klass_name='Panel5D',
    orders=['replicate', 'vector_size', 'threads', 'function_name', 'metric'],
    slices={'vector_size': 'vector_size', 'threads': 'threads',
                 'function_name': 'function_name',
                 'metric': 'metric'},
    slicer=pd.core.panel4d.Panel4D,
    aliases={'major': 'function_name', 'minor': 'metric'},
    stat_axis=2)


Panel5D = pd.core.panelnd.create_nd_panel_factory(
    klass_name   = 'Panel5D',
    orders  = [ 'cool', 'labels','items','major_axis','minor_axis'],
    slices  = { 'labels' : 'labels', 'items' : 'items',
                'major_axis' : 'major_axis', 'minor_axis' : 'minor_axis' },
    slicer  = pd.core.panel4d.Panel4D,
    aliases = { 'major' : 'major_axis', 'minor' : 'minor_axis' },
    stat_axis    = 2)


def prepare_panels(expname):
    header = ['%Time', 'Exclusive msec', 'Inclusive total msec', '#Call', '#Subrs', 'Inclusive usec/call', 'Name']
    replicate_panels = defaultdict(dict)

    for r in REPLICATES:
        data = defaultdict(dict)
        for v in VECTOR_SIZE:
            for t in THREADS:
                tauprofile = "../workdir/{}/r{}/{}/{}/tauprofile".format(expname, r, v, t)
                header_started = None
                tv_data = defaultdict(list)
                with open(tauprofile, 'r') as f:
                    for line in f:
                        if line.startswith('-----') and header_started is None:
                            header_started = True
                        elif line.startswith('-----') and header_started:
                            break
                    for line in f:
                        function_data = line[:-1].strip().split()
                        if len(function_data) > len(header):
                            function_data = function_data[:len(header) - 1] + [" ".join(function_data[len(header) - 1:])]
                        for metric, value in zip(header, function_data):
                            if not value.isalpha():
                                try:
                                    value = int(value)
                                except ValueError:
                                    try:
                                        value = float(value)
                                    except ValueError:
                                        pass
                            tv_data[metric].append(value)
                    data[v][t] = pd.DataFrame(tv_data, index=tv_data['Name'])
        replicate_panels[r] = pd.Panel4D(data)
    return replicate_panels


def prepare_panel(expname, vector_sizes=[10, 100, 1000, 10000, 100000], threads=[1, 2, 4, 8, 10, 16, 20]):
    header = ['%Time', 'Exclusive msec', 'Inclusive total msec', '#Call', '#Subrs', 'Inclusive usec/call', 'Name']
    data = defaultdict(dict)

    for v in vector_sizes:
        for t in threads:
            tauprofile = "../workdir/{}/{}/{}/tauprofile".format(expname, v, t)
            header_started = None
            tv_data = defaultdict(list)
            with open(tauprofile) as f:
                for line in f:
                    if line.startswith('-----') and header_started is None:
                        header_started = True
                    elif line.startswith('-----') and header_started:
                        break
                for line in f:
                    function_data = line[:-1].strip().split()
                    if len(function_data) > len(header):
                        function_data = function_data[:len(header) - 1] + [" ".join(function_data[len(header) - 1:])]
                    for metric, value in zip(header, function_data):
                        if not value.isalpha():
                            try:
                                value = int(value)
                            except ValueError:
                                try:
                                    value = float(value)
                                except ValueError:
                                    pass
                        tv_data[metric].append(value)
            data[v][t] = pd.DataFrame(tv_data, index=tv_data['Name'])
    return data
