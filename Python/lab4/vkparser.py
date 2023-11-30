
import vk

def get_members(groupid):
    first = vk_api.groups.getMembers(group_id=groupid, v=5.131)  # Первое выполнение метода
    data = first["items"]  # Присваиваем переменной первую тысячу id'шников
    count = first["count"] // 1000
    for i in range(1, count+1):  
        data = data + vk_api.groups.getMembers(group_id=groupid, v=5.131, offset=i*1000)["items"]
    return data

if __name__ == "__main__": 
    token = "27b4b5f427b4b5f427b4b5f4f527ce18f3227b427b4b5f44679163a213a9cf49d06af70"
    session = vk.Session(access_token=token)
    vk_api = vk.API(session)

    groupid = input("ID группы: ")
    data = get_members(groupid)
    ids = [ str(x) for x in data ]
    with open(r'members.txt', 'w') as file:
        for item in ids:
            file.write(f"{item},")
    file.close()
    #print(data)
    #usid = data[1]
    #print(vk_api.users.get(user_id = usid, fields = 'nickname', v=5.131))
