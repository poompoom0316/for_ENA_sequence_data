import pandas as pd
from urllib import request
from bs4 import BeautifulSoup
import numpy as np
import copy

base_path = "xxxx/search"


def get_cultivar_list():
    url = f"{base_path}/ine.cgi?action=hinsyu_table&getRow=2000&suito=2&uruchi=0&sankou=ON&chihou=ALL&chihouetc=SOR&kentan=ON&hinmeikana=ON&rikuto=ON&keitocode=ON&ikuseinen=ON&haifunen=ON&ikuseichi=ON&kouhai=ON&order1=0&order2=0&order3=0&rowid=1"

    df_cultivar = pd.read_html(url)[1]

    # get html
    html = request.urlopen(url)
    soup = BeautifulSoup(html, "html.parser")
    inecode_pre = soup.find_all("a", class_="top1", target="_blank")
    inecode_use = pd.DataFrame([[i.attrs["href"].split("ineCode=")[1], i.contents[0].replace("\n\xa0", "")]
                                for i in inecode_pre], columns=["ineCode", '地方番号'])

    df_cultivar_res = pd.merge(inecode_use, df_cultivar, on="地方番号", how="right")

    return df_cultivar_res


def parse_koshu(inecode):
    # inecode = "SAI02500"
    print(inecode)

    seiiku_url = f"{base_path}/ine.cgi?action=seiiku&ineCode={inecode}"
    yield_url = f"{base_path}/ine.cgi?action=syukakubutsu&ineCode={inecode}"
    koshu_url = f"{base_path}/ine.cgi?action=seisankousyu&ineCode={inecode}"

    # 生育データのパース
    seiiku_html = request.urlopen(seiiku_url)
    seiiku_soup = BeautifulSoup(seiiku_html, "html.parser")

    # koshu
    koshu_html = request.urlopen(koshu_url)
    koshu_soup = BeautifulSoup(koshu_html, "html.parser")
    koshu_pre = koshu_soup.find_all("span", class_="s")

    # whethre
    flag1 = len(seiiku_soup.find_all("table", class_="table1")) > 0
    flag2 = len(koshu_soup.find_all("table", class_="table1")) > 0
    flag3 = inecode != "HOK01870"
    flag4 = inecode == "KAN02600"

    if flag1 & flag2 & flag3:
        print("#" * 7 + "Now downloading data" + "#" * 7)
        seiiku_pre = seiiku_soup.find_all("span", class_="s")
        seiiku_pre_parsed = pd.Series([i.contents[0] for i in seiiku_pre])

        df_s = pd.read_html(seiiku_url)
        df_s_use_pos = np.arange(len(df_s))[np.array([i.shape[1] for i in df_s]) > 1].astype(int)

        df_s_use = [df_s[i] for i in df_s_use_pos]
        unit = len(seiiku_pre) // len(df_s_use)
        # seiiku_place = [seiiku_pre[unit * i].contents[0].replace("\u3000\xa0", "").replace("\xa0\xa0\xa0", ", ")
        #                 for i, num in enumerate(df_s_use_pos)]
        seiiku_place_pre = seiiku_pre_parsed.loc[~seiiku_pre_parsed.str.contains("階級区分の表示|注.*")]
        seiiku_place = seiiku_place_pre.str.replace("\u3000\xa0", "").str.replace("\xa0\xa0\xa0",
                                                                                  ", ").values  # seiiku_placeごとにデータをまとめるべき
        seiiku_notion_pre = seiiku_pre_parsed.loc[seiiku_pre_parsed.str.contains("注.*")]

        df_koshu = pd.read_html(koshu_url)
        if flag4:
            # remove data if inecode was "KAN02600"
            df_koshu.pop(3)

        ids = [f"{inecode}_{i}" for i in range(len(df_s_use))]

        # 収量データのパース
        df_y = pd.read_html(yield_url)
        df_y_use_pos = np.arange(len(df_y))[np.array([i.shape[1] for i in df_y]) > 1].astype(int)
        df_y_use = [df_y[i] for i in df_y_use_pos]
        # seiiku_html = request.urlopen(seiiku_url)
        # seiiku_soup = BeautifulSoup(seiiku_html, "html.parser")
        # seiiku_pre = seiiku_soup.find_all("span", class_="s")

        for i in range(len(df_koshu)):
            df_koshu[i]["place_tratment"] = seiiku_place[i]
            df_koshu[i]["var_trial_id"] = ids[i]

            df_s_use[i]["place_tratment"] = seiiku_place[i]
            df_s_use[i]["var_trial_id"] = ids[i]
            df_s_use[i] = df_s_use[i].loc[~df_s_use[i]['試験年次'].str.contains("平均")]

            df_y_use[i]["place_tratment"] = seiiku_place[i]
            df_y_use[i]["var_trial_id"] = ids[i]
            df_y_use[i] = df_y_use[i].loc[~df_y_use[i]['試験年次'].str.contains("平均")]

        return {"seiiku_place": seiiku_place, "koshu": df_koshu, "seiiku": df_s_use, "yield": df_y_use}

    else:
        return None


def concat_total_data():
    df_cultivar_list = get_cultivar_list()

    inecodes = df_cultivar_list["ineCode"].loc[~df_cultivar_list["ineCode"].isnull()].unique()

    # total_dfs = [parse_koshu(inecode) for inecode in inecodes]
    total_dfs = []
    inecode_use = []
    for inecode in inecodes[0:300]:
        # HOK01930(どちらかが多い) KAN02600(ほ場が重複している)
        res = parse_koshu(inecode)
        if res is None:
            print("skipped")
        else:
            total_dfs.append(res)
            inecode_use.append(inecode)

    li = ["seiiku_place", "koshu", "seiiku", "yield"]
    total_dfs_use2 = copy.deepcopy(total_dfs)
    res_tot = {keyi: [i[keyi] for i in total_dfs_use2] for keyi in li}

    seiiku_place = set([element for lis in res_tot["seiiku_place"] for element in lis])
    seiiku_place_seelcted = set([i.split(", ")[1] if "その他" in i else i.split(", ")[0] for i in seiiku_place])

    koshu_df = pd.concat([pd.concat(i) for i in res_tot["koshu"]])
    koshu_df_unique = koshu_df.loc[:,koshu_df.columns[koshu_df.columns!='var_trial_id']].drop_duplicates()

    seiiku_df = pd.concat([pd.concat(i) for i in res_tot["seiiku"]])
    seiiku_df_use = seiiku_df.loc[~seiiku_df["試験年次"].str.contains("平均")]

    ret = geocoder.osm('農研機構東北農業研究センター', timeout=5.0)



