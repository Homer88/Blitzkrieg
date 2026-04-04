# Сборка проекта Blitzkrieg

## Требования

- **CMake** версии 3.0 или выше
- **Visual Studio** (2013, 2015, 2017, 2019 или 2022)
- **DirectX 8 SDK** (уже включен в `../sdk/directx`)

## Быстрая сборка

### Для Windows x86 (32-bit)
```batch
configure_x86.bat
```
Создаст решение в папке `build_x86\Blitzkrieg.sln`

### Для Windows x64 (64-bit)
```batch
configure_x64.bat
```
Создаст решение в папке `build_x64\Blitzkrieg.sln`

### Интерактивный выбор
```batch
generate_vs_project.bat
```
Позволяет выбрать версию Visual Studio

## Ручная сборка

### Visual Studio 2022 (x86)
```batch
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A Win32 ..
```

### Visual Studio 2019 (x86)
```batch
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A Win32 ..
```

### Visual Studio 2022 (x64)
```batch
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
```

## Структура проекта

Проект Blitzkrieg состоит из следующих модулей:

- **GameTT** - основной модуль игровой логики (DLL)
- **AILogic** - искусственный интеллект
- **Anim** - система анимации
- **GameSpy** - сетевой код GameSpy
- **UI** - пользовательский интерфейс
- **Misc** - различные утилиты
- **StreamIO** - ввод-вывод
- **Net** - сетевой модуль
- **libpng** - работа с PNG изображениями
- **LuaLib** - интеграция Lua
- **zlib** - сжатие данных
- **AutoRun** - автозапуск

## SDK

Проект использует DirectX 8 SDK, который находится в:
- Include: `Blitzkrieg\Sources\sdk\directx\include`
- Libraries: `Blitzkrieg\Sources\sdk\directx\lib`

## Совместимость

Проект настроен для компиляции с:
- Стандарт C++98
- `_WIN32_WINNT=0x0400` (Windows 98)
- Поддержкой старых версий Windows (98/2000)

## Устранение проблем

### CMake не находит DirectX8 SDK
Убедитесь, что папка `sdk/directx` существует на уровень выше директории `src`:
```
Blitzkrieg/
  ├── Sources/
      ├── src/         <- вы здесь
      └── sdk/
          └── directx/
              ├── include/
              └── lib/
```

### Ошибка генерации проекта
- Убедитесь, что Visual Studio установлен
- Проверьте, что CMake доступен в PATH
- Запустите `cmake --version` для проверки

## Дополнительные опции сборки

Можно задать при конфигурации:
```batch
cmake -DWIN9X=ON ..           # Для Windows 98
cmake -DWINX86D9=ON ..        # Для DirectX 9 (x86)
cmake -DWINX64D11=ON ..       # Для DirectX 11 (x64)
```
