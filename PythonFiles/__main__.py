import numpy as np
import pandas as pd
import os

if __name__ == '__main__':

    method_data: dict = dict()

    app_path: str = os.getcwd()+r"/../x64/Release/HoldemAgent.exe"
    print(app_path)
    os.system(app_path)

    data_directory: str = r"Data/"
    for filename in os.listdir(data_directory):
        if filename.endswith(".dat"):
            method_name: str = filename[filename.find("_mth-"):filename.find(".dat")]
            data = pd.read_csv(data_directory+filename)
            method_data[method_name] = data
            print(method_name, data.head(), sep="\n")

