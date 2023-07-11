# Минимизация ДКА

Необходимо скачать [предоставленное окружение] и реализовать функцию dfa_minim в файле task.cpp, принимающую в качестве аргумента детерминированный конечный автомат в виде DFA и возвращающую автомат с минимальным количеством состояний, задающий тот же язык, что и исходный автомат.

Окружение содержит реализации алфавита в виде класса Alphabet (этот класс позволяет автоматически получать алфавит, над которым задан автомат) и ДКА в виде класса DFA (этот класс помогает считать автомат в правильном формате). API этих классов приведен в файле api.hpp. Состояния в DFA идентифицируются непустыми строковыми именами, присваиваемыми им при создании.

Тестовые автоматы всегда корректны, детерминированы и задаются в следующем формате: на первой строке перечисляются все символы алфавита, затем на отдельных строчках перечисляются все состояния в формате "[имя_состояния]" или же "[[имя_состояния]]" для финальных состояний. Первое упомянутое состояние считается начальным. Затем перечисляются все переходы в автомате в формате "[имя_начального_состояния] символ_перехода [имя_конечного_состояния]", по одному переходу на строчку. Можно считать, что тестовые автоматы содержат не более 100 состояний.

Функция main в файле main.cpp считывает ДКА из файла dfa_minim.in, передает его в функцию dfa_minim, после чего выводит минимизированный автомат в текстовом виде в файл dfa_minim.out.

Сборка окружения выполняется с помощью CMake или bash-скрипта quick-setup.sh. Получаемый бинарный исполняемый файл - dfa_minim.

[предоставленное окружение]: https://earth.ispras.ru/public-archives/automatoncourse-2022/dfa_minim.zip
