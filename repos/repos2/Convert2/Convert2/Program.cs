using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace Convert2
{
    class Program
    { 
        static string Task2(string jsonname)
        {
            var sr = new StreamReader(jsonname);
            string json = "";
            string tmp;
            while ((tmp = sr.ReadLine()) != null)
            {
                json += tmp.Trim();
            }

            bool ifString(string str)
            {
                char x = str[0];
                char y = str[^1];
                if (str[0] == '"' && str[^1] == '"') 
                    return true;
                return false;
            }

            string ParseObj(string json, ref Stack<string> spaces)
            {
                json = json.Replace(" : ", ":").Replace(": ", ":")
                        .Replace(" :", ":").Trim(new char[] { '{', '}' });
                Console.WriteLine(json);
                List<string> pairs;
                string res = "";
                pairs = new List<string>(json.Split(","));
                for (int i = 0; i < pairs.Count; i++)
                {
                    var pair = pairs[i];
                    //Console.WriteLine(pair);
                    List<string> nodes = new(pair.Split(":"));
                    string nodename = nodes[0].Trim(new char[] { '"', ' ' });
                    nodes[1] = nodes[1].Trim(' ');

                    if (ifString(nodes[1]))
                    {
                        res += $"{spaces.Peek()}<{nodename}>";
                        res += $"{nodes[1].Trim('"')}";
                        res += $"</{nodename}>\n";
                    }
                    else if (nodes[1].StartsWith('{'))
                    {
                        string objstr = $"{nodes[1]}:{nodes[2]}";
                        Stack<char> st = new();
                        st.Push('{');
                        while (true)
                        {
                            i++;
                            objstr += $",{pairs[i]}";
                            if (pairs[i].StartsWith('{'))
                            {
                                st.Push('{');
                            }
                            if (pairs[i].EndsWith('}')) {
                                st.Pop();
                                if (st.Count == 0) break;
                            }
                        }
                        string name = nodes[0].Trim('"');
                        res += $"{spaces.Peek()}<{name}>\n";
                        spaces.Push(spaces.Peek() + "  ");
                        res += ParseObj(objstr, ref spaces);
                        spaces.Pop();
                        res += $"{spaces.Peek()}</{name}>\n";
                    }
                    else if (nodes[1].StartsWith('['))
                    {
                        string str;
                        string arrname = nodes[0].Trim('"');
                        string objstr = $"{nodes[1]}:{nodes[2]}";
                        Stack<char> st = new();
                        st.Push('{');
                        while (true) {
                            str = pairs[++i];
                            objstr += $",{str}";
                            if (str.StartsWith('{'))
                            {
                                st.Push('{');
                            }
                            if (str.Trim(']').EndsWith('}'))
                            {
                                st.Pop();
                                
                                if (st.Count == 0)
                                {
                                    res += $"{spaces.Peek()}<{arrname}>\n";
                                    spaces.Push(spaces.Peek() + "  ");
                                    res += ParseObj(objstr.Trim(new char[]{ '[', ']', ','}), ref spaces);
                                    spaces.Pop();
                                    res += $"{spaces.Peek()}</{arrname}>\n";
                                }
                                objstr = "";
                            }
                            if (str.EndsWith(']'))
                                break;
                        }
                    }
                    else if (nodes[1] == "null")
                    {
                        res += $"{spaces.Peek()}<{nodename}/>\n";
                    }
                    else
                    {
                        res += $"{spaces.Peek()}<{nodename}>";
                        res += $"{nodes[1]}";
                        res += $"</{nodename}>\n";
                    }
                    //Console.WriteLine();
                }
                return res;
            }

            if (json.StartsWith('[') && json.EndsWith(']'))
            {
                json = "\"row\":" + json;
            }

            Stack<string> spaces = new();
            spaces.Push("  ");
            string res = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<root>\n";
            res += ParseObj(json, ref spaces);
            res += "</root>";
            //Console.Write(res);

            using (StreamWriter sw = new(@"D:\JSON\out.xml"))
            {
                sw.Write(res);
            }
            return res;
        }
        
        static void Main(string[] args)
        {
            string jsonname = @"D:\JSON\sample2.json";//Console.ReadLine();
            Task2(jsonname);

            string original = "", content = "";
            var lines = File.ReadAllLines(@"D:\JSON\out.xml");
            content = String.Join("", lines[2..(lines.Length-1)]);
            using (StreamReader sr = new(@"D:\JSON\out.xml")) {
                //content = sr.Read//ReadToEnd();
            };
            using (StreamReader sr = new(@"D:\JSON\o2.xml"))
            {
                original = sr.ReadToEnd();
            };
            content = content.Trim(' ');
            var col = new List<char>(content);
            Regex reg = new(@">[\s]+<");
            Console.WriteLine(reg.Replace(content, ">\n<"));
            Console.WriteLine();
            //Console.WriteLine(reg.Replace(original, ">\n<"));
        }
    }
}
