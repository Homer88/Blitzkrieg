# Исправления AILogic для компиляции на MSVC 2022

**Дата:** 08.04.2026
**Статус:** ✅ Успешно собрано (AILogic.dll)

## Список исправлений

### 1. AIHashFuncs.h - SUnitObjHash и неполный тип CAIUnit
**Проблема:** `SUnitObjHash` использовал `CObj<CAIUnit>`, но `CAIUnit` был только forward declared.
**Решение:** `SUnitObjHash` уже был перенесён в `AIUnit.h` (после полного определения `CAIUnit`).

### 2. push_back()/push_front() без аргументов
**Проблема:** В C++98 `push_back()` создавал элемент по умолчанию. В C++11+ это не работает.
**Решение:** Заменено на `emplace_back()`/`emplace_front()`:
- `AIWarFog.cpp` (строка 305)
- `AIStaticMap.cpp` (строка 1274)
- `RotatingFireplacesObject.cpp` (строка 17)
- `GroupLogic.cpp` (строка 312)
- `UnitGuns.cpp` (строки 100, 162)

### 3. CAnimUnit не определён
**Проблема:** `BASIC_REGISTER_CLASS(CAnimUnit)` использовался, но класс `CAnimUnit` не был определён.
**Решение:** Создан полный класс `CAnimUnit` в `AnimUnit.h` с:
- Конструктор по умолчанию
- Конструктор с параметром `CAIUnit*`
- Поля: `pOwner`, `bTechnics`, `nCurAnimation`, `timeOfFinishAnimation`, `movingState`
- Реализация методов в `AnimUnit.cpp`
- Добавлен `AnimationSet(int)` как обёртка для `AnimationSet(int, int)`

### 4. CAIUnitData не определён в Graveyard.cpp
**Проблема:** Использование `CAIUnitData` через placement new, но тип не определён.
**Решение:** Заменено на `SAINotifyDeadAtAll` (строки 123, 138).

### 5. Неоднозначный оператор `<` для SVector
**Проблема:** `SVectorHash::operator()` использовал `a < b`, что создавало неоднозначность.
**Решение:** Явное сравнение полей:
```cpp
return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y);
```
Исправлено в:
- `GeneralIntendant.h` (строка 197)
- `RectTiles.cpp` (строка 179)

### 6. IGun не определён
**Проблема:** `IGun* GetGun()` возвращал `CBasicGun*`, но `IGun` не был определён.
**Решение:** Добавлен интерфейс `IGun` в `AILogicMissingTypes.h`:
```cpp
interface IGun : public IRefCount { };
```

### 7. GunDescriptorInternal - преобразование типов
**Проблема:** `CBasicGun*` не мог быть преобразован в `IGun*`.
**Решение:** Использован `reinterpret_cast<IGun*>()` в `GunDescriptorInternal.cpp`.

### 8. UnitCreation - инициализация CTreeAccessor
**Проблема:** `CTreeAccessor tree = CreateDataTreeSaver(...)` - MSVC 2022 не допускает копирование.
**Решение:** Прямой вызов конструктора:
```cpp
CTreeAccessor tree( CreateDataTreeSaver( pStream, IDataTree::READ ) );
```

### 9. CastToRefCountImpl - дубликаты специализаций
**Проблема:** `BASIC_REGISTER_CLASS` создавал дубликаты явных специализаций шаблонов (error C2908).
**Решение:** Отключён PCH для файлов с `BASIC_REGISTER_CLASS`:
```cmake
set_source_files_properties(
    Aviation.cpp Formation.cpp PathFinderInternal.cpp
    ShootEstimatorInternal.cpp StandartPath.cpp
    Statistics.cpp UnitStates.cpp
    PROPERTIES COMPILE_FLAGS "/Y-"
)
```

### 10. CMakeLists.txt - файлы из подпапок
**Проблема:** `FILE(GLOB "*.cpp")` не находил файлы в подпапке `Scripts/`.
**Решение:**
```cmake
FILE(GLOB AILOGIC_SOURCE "*.cpp" "Scripts/*.cpp")
FILE(GLOB AILOGIC_HEADERS "*.h" "Scripts/*.h")
```

### 11. Отключены предупреждения для MSVC 2022
```cmake
target_compile_options(AILogic PRIVATE /wd4430)
```

## Итоговый результат
✅ **AILogic.dll успешно собран** в `build/AILogic/Release/AILogic.dll`
