from bs4 import BeautifulSoup as bs
import requests as req

HEADERS = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36',
    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
    'Accept-Language':'ru-ru,ru;q=0.8,en-us;q=0.5,en;q=0.3',
    'Accept-Encoding':'gzip, deflate',
    'Connection':'keep-alive',
    'DNT':'1'
}

url = input("Введите url группы в Одноклассниках: ")
url = f"https://ok.ru{url}"
session = req.session()
page = session.get(url, headers=HEADERS)

#with open('page.html',"wb") as file:
    #file.write(page.content)
soup = bs(page.text, 'html.parser')


div = soup.find(id='hook_Block_AltGroupMainMembersRB')
links = div.find_all('a')
target = ""
for link in links:
    href = link.get('href')
    if '/members' in link.get('href'):
        target = href

print('ссылка на подписчиков:', target)

url = f"https://ok.ru{target}"
page = session.get(url, headers=HEADERS)

soup = bs(page.text, 'html.parser')
divs = soup.find_all("div", class_='ellip')
#for div in divs:
    #print(div.a.string)

scripts = soup.head.find_all("script")
for scr in scripts:
    src = scr.get("src")
    print(src)
#print(scripts)
    
