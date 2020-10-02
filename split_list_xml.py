import xmltodict
import pandas as pd


def main(filepath, outpath):
    fd = open(filepath)
    doc = xmltodict.parse(fd.read())
    print(doc)

    doc_df = pd.DataFrame(doc['SAMPLE_SET']["SAMPLE"]).loc[:, ['@accession', '@alias', '@broker_name', 'TITLE']]

    return qq

if __name__ == '__main__':
    filepath = "Data/hogehoge.xml"
    outpath = "Analysis/hogehoge.xml"
    main(filepath, outpath)