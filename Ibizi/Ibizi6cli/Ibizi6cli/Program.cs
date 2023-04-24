using Dict = System.Collections.Generic.Dictionary<int,
    System.Collections.Generic.List<Privilege>>;

void Main()
{
    int x = 0, curUser = -1, ptr = -1, admin = -1;
    List<User> users = new() { new(x++, "Roma", true, "1234"), 
                               new(x++, "Anya"), 
			       new(x++, "Nastya"), 
                               new(x++, "Dima") };
    List<Object> objects = new() { new(x++, "One", "Some Text"), 
				   new(x++, "Two"), 
                                   new(x++, "Three"), 
				   new(x++, "Four") };
    var access = new Dictionary<int, Dict>();

    InitAccessMatrix();

    bool exit = false;
    while (!exit)
    {
        Console.Write("> ");
        string command = Console.ReadLine();
        var arr = command?.Split(" ")
            .Where(x => !string.IsNullOrEmpty(x)).ToArray();
        switch (arr?[0])
        {
            case "login": {
                    string un = arr.Length > 1 ? arr[1] : "";
                    string pwd = arr.Length > 2 ? arr[2] : "";
                    Login(un, pwd);
                    break; }
            case "check":
                {
                    string fname = arr.Length > 1 ? arr[1] : "";
                    Check(fname);
                    break;
                }
            case "read":
                {
                    string fname = arr.Length > 1 ? arr[1] : "";
                    Read(fname);
                    break;
                }
            case "write":
                {
                    string fname = arr.Length > 1 ? arr[1] : "";
                    string content = arr.Length > 2 ? arr[2] : "";
                    Write(fname, content);
                    break;
                }
            case "grant":
                {
                    string fname = arr.Length > 1 ? arr[1] : "";
                    string uname = arr.Length > 2 ? arr[2] : "";
                    Grant(fname, uname);
                    break;
                }
            case "logout":
                {
                    Logout();
                    break;
                }
            case "exit":
                {
                    exit = true;
                    break;
                }
            default: exit = true; break;
        }
    }

    void Read(string fname)
    {
        User user = users.FirstOrDefault(x => x.Id == curUser);
        if (user == null)
        {
            Console.WriteLine("Не авторизованы:(");
            return;
        }
        Object obj = objects.FirstOrDefault(x => x.Name == fname);
        if (obj == null)
        {
            Console.WriteLine("Нет такого объекта:(");
            return;
        }
        var priv = access[curUser][obj.Id];
        if (!priv.Contains(Privilege.Read))
        {
            Console.WriteLine($"Нет прав на чтение объекта {fname}");
            return;
        }
        Console.WriteLine($"Содержимое объекта {fname}: ({obj.Content})");
    }

    void Write(string fname, string content)
    {
        User user = users.FirstOrDefault(x => x.Id == curUser);
        if (user == null)
        {
            Console.WriteLine("Не авторизованы:(");
            return;
        }
        Object obj = objects.FirstOrDefault(x => x.Name == fname);
        if (obj == null)
        {
            Console.WriteLine("Нет такого объекта:(");
            return;
        }
        var priv = access[curUser][obj.Id];
        if (!priv.Contains(Privilege.Write))
        {
            Console.WriteLine($"Нет прав на запись в объект {fname}");
            return;
        }
        objects.Remove(obj);
        var newObj = obj with { Content = content };
        objects.Add(newObj);
        Console.WriteLine($"Содержимое объекта {fname}: ({newObj.Content})");
    }

    void Grant(string fname, string uname, string privStr = "read")
    {
        User user = users.FirstOrDefault(x => x.Id == curUser);
        if (user == null)
        {
            Console.WriteLine("Не авторизованы:(");
            return;
        }
        Object obj = objects.FirstOrDefault(x => x.Name == fname);
        if (obj == null)
        {
            Console.WriteLine("Нет такого объекта:(");
            return;
        }
        var privs = access[curUser][obj.Id];
        if (!privs.Contains(Privilege.Grant))
        {
            Console.WriteLine($"Нет прав назначения доступа к объекту {fname}");
            return;
        }
        User user2 = users.FirstOrDefault(x => x.Name == uname);
        if (user2 == null)
        {
            Console.WriteLine("Нет такого пользователя:(");
            return;
        }
        Privilege priv = Privilege.Read;
        switch (privStr)
        {
            case "read": { priv = Privilege.Read; break; }
            case "write": { priv = Privilege.Write; break; }
            case "grant": { priv = Privilege.Grant; break; }
            default:
                Console.WriteLine("Нет таких прав:(");
                break;
        }
        var arr = access[user2.Id][obj.Id];
        if (!arr.Contains(priv))
            arr.Add(priv);

        Console.WriteLine($"Права {privStr} на объект {fname} выданы {uname}");
    }

    void Logout()
    {
        curUser = -1;
        Console.WriteLine("До свидания...");
    }

    void Login(string uname, string pwd)
    {
        User user = users.FirstOrDefault(x => x.Name == uname);
        if (user == null)
        {
            Console.WriteLine("Нет такого пользователя:(");
            return;
        }
        if (!string.IsNullOrEmpty(user.Password) 
            && user.Password != pwd)
        {
            Console.WriteLine("Неверный пароль:(");
            return;
        }
        Console.WriteLine($"Успешная авторизация как {uname}!");
        curUser = user.Id;
    }

    void Check(string oname)
    {
        User user = users.FirstOrDefault(x => x.Id == curUser);
        if (user == null)
        {
            Console.WriteLine("Не авторизованы:(");
            return;
        }

        if (curUser == admin)
        {
            PrintAccessMatrix();
            return;
        }

        if (string.IsNullOrEmpty(oname))
        {
            foreach (var ob in objects)
            {
                var prvs = access[user.Id][ob.Id];
                string rr = prvs.Contains(Privilege.Read) ? Privilege.Read.ToString() : "";
                string ww = prvs.Contains(Privilege.Write) ? Privilege.Write.ToString() : "";
                string gg = prvs.Contains(Privilege.Grant) ? Privilege.Grant.ToString() : "";
                var res = $"права ({rr} {ww} {gg}) на объект {ob.Name}";
                Console.WriteLine(res);
            }
            return;
        }
        var obj = objects.FirstOrDefault(x => x.Name == oname);
        if (obj == null)
        {
            Console.WriteLine("Нет такого объекта:(");
            return;
        }
        var privs = access[user.Id][obj.Id];
        string r = privs.Contains(Privilege.Read) ? Privilege.Read.ToString() : "";
        string w = privs.Contains(Privilege.Write) ? Privilege.Write.ToString() : "";
        string g = privs.Contains(Privilege.Grant) ? Privilege.Grant.ToString() : "";
        var str = $"права ({r} {w} {g}) на объект {oname}";
        Console.WriteLine(str);
    }

    void InitAccessMatrix()
    {
        // инициализация матрицы доступа
        curUser = users.First(x => x.Name == "Roma").Id;
        access.Add(curUser, new Dict());
        foreach (var obj in objects)
            access[curUser].Add(obj.Id,
                new() { Privilege.Read, Privilege.Write, Privilege.Grant });
        admin = curUser;

        curUser = users.First(x => x.Name == "Anya").Id;
        ptr = objects.First(x => x.Name == "One").Id;
        access.Add(curUser, new Dict());
        foreach (var obj in objects)
        {
            access[curUser].Add(obj.Id, new() { Privilege.Read, Privilege.Write });
            access[curUser][ptr].Add(Privilege.Grant);
        }

        curUser = users.First(x => x.Name == "Dima").Id;
        access.Add(curUser, new Dict());
        foreach (var obj in objects)
            access[curUser].Add(obj.Id, new() { Privilege.Read });

        curUser = users.First(x => x.Name == "Nastya").Id;
        access.Add(curUser, new Dict());
        foreach (var obj in objects)
            access[curUser].Add(obj.Id, new());

        curUser = -1;
    }

    void PrintAccessMatrix()
    {
        Console.Write(string.Format("{0,10}", ""));
        foreach (var obj in objects)
        {
            Console.Write(string.Format("{0,10}", obj.Name));
        }
        Console.WriteLine();
        foreach (int uid in access.Keys)
        {
            string uname = users.First(x => x.Id == uid).Name;
            Console.Write(string.Format("{0,10}", uname));
            var str = "";
            foreach (var ob in objects)
            {
                var privs = access[uid][ob.Id];
                int? r = privs.Contains(Privilege.Read) ? (int)Privilege.Read : null;
                int? w = privs.Contains(Privilege.Write) ? (int)Privilege.Write : null;
                int? g = privs.Contains(Privilege.Grant) ? (int)Privilege.Grant : null;
                str = $"{r}{w}{g}";
                Console.Write(string.Format("{0,10}", str));
            }
            Console.WriteLine();
        }

    }
}

Main();

public record User(int Id, string Name, bool IsAdmin = false, string Password = "");

public record Object(int Id, string Name, string Content = "");

enum Privilege { /*None = 0,*/ Write = 1, Read = 2, Grant = 3 }