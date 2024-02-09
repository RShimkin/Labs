
val dff = sc.textFile("C:\\OOV\\Laba\\people.txt").map(_.split(" "))
	.map(x => (x(0).trim.toInt, x(1).trim, x(2).trim.toInt, x(4).trim, x(5).trim, x(6).trim.toInt))
	.toDF("class", "name", "age", "gender", "subject", "result")
	
val dff = sc.textFile("C:\\OOV\\Laba\\people.txt").map(_.split(" ")).map(x => (x(0).trim.toInt, x(1).trim, x(2).trim.toInt, x(4).trim, x(5).trim, x(6).trim.toInt)).toDF("class", "name", "age", "gender", "subject", "result")

//dff.printSchema()
dff.show()

val res1 = dff.select("name").distinct().count()
println("1: " + res1)
val res11 = dff.select("name").distinct().filter("age<20").count()
println("1.1: " + res11)
val res12 = dff.select("name").distinct().filter("age>=20").count()
println("1.2: " + res12)
val res13 = dff.select("name").distinct().filter("age>20").count()
println("1.3: " + res13)

val res2 = dff.select("name").distinct().filter("gender=='man'").count()
println("2: " + res2)
val res21 = dff.select("name").distinct().filter("gender=='women'").count()
println("2.1: " + res21)

val res3 = dff.select("name").distinct().filter("class==12").count()
println("3: " + res3)

println("4:")
val res4 = dff.filter("subject<>'mathematics'").agg(avg("result")).show()
println("4.1:")
val res41 = dff.filter("subject=='mathematics'").agg(avg("result")).show()
println("4.2:")
val res42 = dff.filter("subject=='еnglish'").agg(avg("result")).show()

println("5:")
val res5 = dff.agg(avg("result")).show()

println("6:")
val res6 = dff.filter("class==12").agg(avg("result")).show()
println("6.1:")
val res61 = dff.filter("class==12").filter("gender=='man'").agg(avg("result"))
println("6.2:")
val res62 = dff.filter("class==12").filter("gender=='women'").agg(avg("result"))

//dff.filter("class==13").agg(avg("result")).show()
//dff.filter("class==13").filter("gender=='man'").agg(avg("result")).show()
//dff.filter("class==13").filter("gender=='women'").agg(avg("result")).show()

println("7:")
val res7 = dff.filter("subject=='сhinese'").groupBy().max("result").show()  
println("7.1:")
val res71 = dff.filter("subject=='сhinese'").filter("class==12").groupBy().min("result").show
println("7.2:")
val res72 = dff.filter("subject=='mathematics'").filter("class==13").groupBy().max("result").show

val res8 = dff.filter("gender=='women'").filter("class==12").groupBy("name").agg(sum("result") as "total").filter("total > 150").count()
println("8: " + res8)

val names1 = dff.filter("subject=='mathematics'").filter("result>=70").select("name")
val names2 = dff.groupBy("name").agg(sum("result") as "average").filter("average>=150").select("name")
println("9:")
val res9 = dff.join(names1, names1("name")===dff("name")).join(names2, names2("name")===dff("name")).select("name","gender",show

