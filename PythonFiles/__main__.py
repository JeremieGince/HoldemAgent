import numpy as np
import pandas as pd
import os
import matplotlib.pyplot as plt
from copy import deepcopy


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
            method_data[method_name] = get_global_data(data)

            # print(method_name, method_data[method_name][["skill_average", "skill balance", "efficiency", ]], '\n', sep="\n")
    return method_data


def get_global_data(data):

    data = deepcopy(data).fillna(0)

    # print("nb_hands.sum: ", data["nb_hands"].sum())
    # print("data.shape: ", data.shape)
    # print(" data['successful_loss']: ", data["successful_loss_rate"] * (data["nb_hands"] - data["expected_win_rate"] * data["nb_hands"]))

    expected_win = data["expected_win_rate"] * data["nb_hands"]
    successful_win = data["successful_win_rate"] * expected_win
    successful_loss = data["successful_loss_rate"] * (data["nb_hands"] - expected_win)
    bluff = data["bluff_rate"] * (data["nb_hands"] - expected_win)

    # print("expected_win: ", expected_win.sum(), " successful_win: ", successful_win.sum(),
    #       " successful_loss: ", successful_loss.sum(),
    #       " bluff: ", bluff.sum())

    expected_win_rate = expected_win.sum() / data["nb_hands"].sum()
    successful_win_rate = successful_win.sum() / expected_win.sum()
    successful_loss_rate = successful_loss.sum() / (data["nb_hands"].sum() - expected_win.sum())
    bluff_rate = bluff.sum() / (data["nb_hands"].sum() - expected_win.sum())

    # print("expected_win_rate: ", expected_win_rate, " successful_win_rate: ", successful_win_rate,
    #       " successful_loss_rate: ", successful_loss_rate, " bluff_rate: ", bluff_rate)

    skill_average = (2/5) * successful_win_rate + (2/5) * successful_loss_rate + (1/5) * bluff_rate
    skill_balance = 1-((4/5) * np.abs(successful_win_rate - successful_loss_rate) + \
                       (1 / 10) * np.abs(bluff_rate - successful_loss_rate) + \
                       (1 / 10) * np.abs(successful_win_rate - bluff_rate))

    new_data = pd.DataFrame(columns=data.columns.to_list())
    new_data = new_data.append({
        "Taux de réussite": data["win_rate"].mean(),
        "Taux de mains gagnantes": expected_win_rate,
        "Taux de victoires réussites": successful_win_rate,
        "Taux de défaites réussites": successful_loss_rate,
        "Taux de bluff": bluff_rate,
        "Performance moyenne": skill_average,
        "Équilibre des capacités": skill_balance,
        "Efficacité": skill_average * skill_balance,
        "Gains": data["gains"].sum(),
        "Pertes": data["loss"].sum(),
        "Nombre de mains": data["nb_hands"].sum(),
        "Durée [s]": data["elapse_time [s]"].sum()
    }, ignore_index=True).iloc[0]

    return new_data


def plot_data(method_data: dict):
    other_stats = ["Taux de réussite", "Taux de mains gagnantes", ]
    base_stats = ["Taux de victoires réussites", "Taux de défaites réussites", "Taux de bluff", ]
    power_stats = ["Performance moyenne", "Équilibre des capacités", "Efficacité", ]

    stats: dict = {
        "other stats": other_stats,
        "base_stats": base_stats,
        "power_stats": power_stats,
    }

    for sts_name, sts in stats.items():
        fig = plt.figure(figsize=(16, 5))
        for i, (mth, data) in enumerate(method_data.items()):
            print(mth, data[sts], '\n', sep='\n')
            data[sts].sort_index().plot.barh(alpha=1.0, label=mth,
                                             color=COLORS[i%len(COLORS)],
                                             align=["center", "edge"][i%2])
        plt.xlim([0, 1])
        plt.legend()
        plt.savefig(rf"Figures/{sts_name}_mths-{'-'.join([mth for mth in method_data])}.png", dpi=500)
        # plt.show(block=False)
        plt.close(fig)


if __name__ == '__main__':
    base_rn_data = load_data("base-rn")
    mcts_base_data = load_data("mcts-base")
    mcts_rn_data = load_data("mcts-rn")
    plot_data(base_rn_data)
    plot_data(mcts_base_data)
    plot_data(mcts_rn_data)
