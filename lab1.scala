val word = "rainbow"
val filename = "C:\\OOV\\Laba\\Shimkin.txt"

val file = sc.textFile(filename)
val words = file.flatMap(_.split("\\P{L}+"))
val grouped = words.map((_,1)).reduceByKey(_ + _)
val found = grouped.filter(x => x._1 == word).collect()
val result = if (found.length > 0) word + "is found " + found(0)._2 + " times" else word + "is not found"
println()
println(result)

//Над файлом произвести следующие действия:
//1. найти заданное слово
//2. посчитать количество вхождений заданного слова;
//3. разбить текст на слова и удалить пустые строки.
//4. Создать наборы RDD на основе массивов (целочисленных и ассоциативных) и применить к ним Transformation reduce и map.