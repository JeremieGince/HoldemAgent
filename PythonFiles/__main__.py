import numpy as np
import pandas as pd
import os
import matplotlib.pyplot as plt


COLORS = ['m', 'c', 'g', 'k', 'b', 'y', 'r', 'w']


def run():
    for _ in range(100):
        app_path: str = os.getcwd() + r"/../x64/Release/HoldemAgent.exe"
        os.system(app_path)


def load_data(TH_game_name: str):
    method_data: dict = dict()

    data_directory: str = os.getcwd()+rf"/../Data/{TH_game_name}/Data/"
    for filename in os.listdir(data_directory):
        if filename.endswith(".dat"):
            method_name: str = filename[filename.find("_mth-")+5:filename.find(".dat")]
            data = pd.read_csv(data_directory + filename)
            data = data.fillna(0)
            method_data[method_name] = data.mean(axis=0)
            print(method_name, data.mean(axis=0), '\n', sep="\n")
    return method_data


def plot_data(method_data: dict):
    other_stats = ["win_rate", "expected_win_rate", ]
    base_stats = ["successful_win_rate", "successful_loss_rate", "bluff_rate", ]
    power_stats = ["skill_average", "skill balance", "efficiency"]

    stats: dict = {
        "other stats": other_stats,
        "base_stats": base_stats,
        "power_stats": power_stats,
    }

    for sts_name, sts in stats.items():
        fig = plt.figure(figsize=(13, 5))
        for i, (mth, data) in enumerate(method_data.items()):
            data[sts].sort_index().plot.barh(alpha=1.0, label=mth,
                                             color=COLORS[i%len(COLORS)],
                                             # width=0.9-i/10,
                                             align=["center", "edge"][i%2])
        plt.xlim([0, 1])
        # plt.xlabel("rate")
        plt.legend()
        plt.savefig(rf"Figures/{sts_name}_mths-{'-'.join([mth for mth in method_data])}.png", dpi=500)
        plt.show(block=False)


if __name__ == '__main__':
    base_rn_data = load_data("base-rn")
    plot_data(base_rn_data)
