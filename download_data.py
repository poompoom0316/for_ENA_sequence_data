from urllib import request
from bs4 import BeautifulSoup
import os


def main():
    url = ""
    response = request.urlopen(url)
    soup = BeautifulSoup(response)
    response.close()

    urls1 = [i.get("href") for i in soup.find_all(class_="btn ")]
    urls2 = [i.get("href") for i in soup.find_all(class_="btn ")]

    for urli in urls1+urls2:
        os.system(f'aria2c -x 16 -s 16 {urli}')


