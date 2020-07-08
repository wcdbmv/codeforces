# [B. Наименее круглый путь](https://codeforces.com/problemset/problem/2/B)

<table>
	<tr>
		<th>ограничение по времени на тест:</th>
		<td>2 seconds</td>
	</tr>
	<tr>
		<th>ограничение по памяти на тест:</th>
		<td>64 megabytes</td>
	</tr>
	<tr>
		<th>ввод:</th>
		<td>стандартный ввод</td>
	</tr>
	<tr>
		<th>вывод:</th>
		<td>стандартный вывод</td>
	</tr>
</table>

Задана квадратная матрица n × n, состоящая из неотрицательных целых чисел.
Вам надо найти такой путь на ней, который
1. начинается в левой верхней ячейке матрицы;
2. каждой следующей ячейкой имеет правую или нижнюю от текущей;
3. заканчивается в правой нижней клетке.

Кроме того, если перемножить все числа вдоль пути и посмотреть на получившиеся произведение, то это число должно быть как можно менее «круглым».
Иными словами оно должно заканчиваться на наименьшее возможное количество нулей.

## Входные данные

В первой строке содержится целое число n (2 ≤ n ≤ 1000), n — размер заданной матрицы.
Далее в n строках содержатся элементы матрицы (целые неотрицательные числа, не превосходящие 10<sup>9</sup>).

## Выходные данные

В первую строку выведите искомое наименьшее количество концевых нулей в произведении чисел вдоль пути.
Во вторую выведите сам путь.

## Примеры

<table>
	<tr>
		<th>входные данные</th>
		<th>выходные данные</th>
	</tr>
	<tr>
		<td><pre>3<br>1 2 3<br>4 5 6<br>7 8 9</pre></td>
		<td><pre>0<br>DDRR</pre></td>
	</tr>
</table>