from selenium import webdriver
from selenium.webdriver.common.keys import Keys
#from selemiumwire import webdriver
#from fake_useragent import UserAgent
import time
from auth_data import vk_email, vk_pass


def main():

    vk_url = "https://vk.com/"
    url = input("Группа: ")

    options = webdriver.ChromeOptions()
    ua = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.118 Safari/537.36"
    options.add_argument(f"user-agent={ua}")
    options.add_argument("--disable-blink-features=AutomationControlled")
    #options.headless = True
    
    driver = webdriver.Chrome(options=options)

    try:
        driver.get(vk_url)

        email_input = driver.find_element_by_id("index_email")
        email_input.clear()
        email_input.send_keys(vk_email)

        pass_input = driver.find_element_by_id("index_pass")
        pass_input.clear()
        pass_input.send_keys(vk_pass)

        driver.find_element_by_id("index_login_button").click()

        time.sleep(5)

        input_field = driver.find_element_by_id("ts_input")
        input_field.clear()
        input_field.send_keys(url)
        input_field.send_keys(Keys.ENTER)

        time.sleep(3)

        links = driver.find_elements_by_css_selector(".author")
        links[0].click()

        time.sleep(3)

        target = None
        links = driver.find_elements_by_css_selector(".module_header")
        target = links[0]
        #for link in links:
            #href = link.get_attribute('href')
            #if "people" in href:
                #target = link
        target.click()

        time.sleep(5)

        container = 'document.querySelector("#box_layer")'
        last_height = driver.execute_script(f"return {container}.scrollHeight")
        print(last_height)
        people = []
        while True:
            for i in range(0,15):
                people = driver.find_elements_by_css_selector(".fans_fan_lnk")
                target = people[-1]
                target.send_keys("")
            new_height = driver.execute_script(f"return {container}.scrollHeight")
            if new_height == last_height:
                break
            else:
                last_height = new_height

        print(len(list(people)))
        #for fan in list(people):
            #name = fan.get_attribute("innerHTML")
            #print(name.replace('<br>'," "))

        
        #SCROLL_PAUSE_TIME = 0.5
        #container = 'document.querySelector("#box_layer")'
        #last_height = driver.execute_script(f"return {container}.scrollHeight")
        #while True:
        #driver.execute_script(f"{container}.scrollTo(0,{last_height});")
        #driver.execute_script(f"window.scrollTop = {last_height}-100;")

        #html = driver.find_element_by_tag_name('html')
        #for i in range(0,100):
            #html.send_keys(Keys.ARROW_DOWN)
        
        
        #driver.current_url
        #driver.switch_to.window(driver.window_handles[0])
        #driver.implicitly_wait(5)
        
        time.sleep(2)
        
    except Exception as ex:
        print(ex)
    finally:
        driver.close()
        driver.quit()

if __name__ == "__main__":
    main()
