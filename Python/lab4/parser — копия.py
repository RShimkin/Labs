from bs4 import BeautifulSoup as bs
import requests as req

def printlist(lst):
    for x in lst:
        print(x)

url = input("Введите url группы Facebook: ")
page = req.get(url)

print(page.status_code)
soup = bs(page.text, 'html.parser')

file = open(r'D:\facebook.html', "wb")
file.write(page.content)
file.close()
body = soup.body
print(body.prettify())
