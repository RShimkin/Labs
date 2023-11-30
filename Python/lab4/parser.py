from bs4 import BeautifulSoup as bs
import requests as req

def printlist(lst):
    for x in lst:
        print(x)

url = input("Введите url группы ВК: ")
url = f"https://vk.com/{url}"
header = {
    'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:45.0) Gecko/20100101 Firefox/45.0'
}
page = req.get(url, headers = header)

print(page.status_code)
#file = open(r'D:\CONTENT3.html', "wb")
#file.write(page.content)
#file.close()
soup = bs(page.text, 'html.parser')
#links = soup.findAll('a')
#refs = [ x.get('href') for x in links ]
#printlist(refs)

div = soup.find(id='public_followers')
links = div.find_all('a')
for x in links:
    href = x.get('href')
    if '[group]' in href:
        link = href
print(link)

url = f"https://vk.com{link}"
page = req.get(url, headers = header)
soup = bs(page.text, 'html.parser')
div = soup.find(id='results')
print(div.prettify())
