# Project for Arizona

## Стек технологий

| Компонент | Описание |
|-----------|----------|
| **open.mp** | Серверная платформа |
| **Pawn.RakNet** | Работа с сетевыми пакетами |
| **Pawn.CMD** | Система команд |
| **sscanf2** | Парсинг строк |
| **foreach** | Итераторы |

## CEF-система

### Svelte (58 компонентов)

InteractionMenu, BattleRoyale, BattleRoyaleInventory, BattleRoyaleHud, BattleRoyaleMap, ArizonaPass, DonationShop, ContainersAuction, FadeScreen, FilmedText, ActionProgressBar, Phone, GasStation, Auth, VideoBackground, AnimationsMenu, Screamer, ActionBar, StreetFood, NpcDialog, HorizontalPhone, Quests, Dice, CarNumbers, Clicker, EmploymentHistory, CrateRoulette, TuningStation, BusinessesList, BusinessInformation, Situations, ContainerContent, Inventory, PropertyList, PropertyInformation, CasinoRoulette, CasinoSlotsMachine, RewardBanner, MountainTestDrive, Truckers, GlobalEventMenu, GlobalEventQuests, RewardsNewYear, Blueprint, InteractiveMenu, Documents, FindGame, BattlePassChose, EventPass, MetalDetector, Excavations, Family, DonateShop, EggsSortGame, DocumentsSortGame, GhettoMap, CrystalsSortGame

### Модальные окна (9 типов)

| Тип | Название |
|-----|----------|
| `MODAL_TYPE_INFORMATION` | information |
| `MODAL_TYPE_BUSINESS_INFO` | businessInfo |
| `MODAL_TYPE_INTERACTION_SIDEBAR` | interactionSidebar |
| `MODAL_TYPE_DIALOG` | dialog |
| `MODAL_TYPE_KEYREACTION` | keyReaction |
| `MODAL_TYPE_DIALOGTIP` | dialogTip |
| `MODAL_TYPE_SECURITY_BANNER` | securityBanner |
| `MODAL_TYPE_GIVEAWAYPANEL` | giveawayPanel |
| `MODAL_TYPE_VEHICLE_MENU` | carMenu |

### UI Elements (64 элемента, Mobile)

| ID | Элемент | ID | Элемент |
|----|---------|----|---------|
| 1 | UI_COMMAND_BINDER | 42 | UI_DONATE_SHOP |
| 2 | UI_VOICE_SETTINGS | 43 | UI_CONTAINER |
| 3 | UI_VOICE_PLAYERS_SETTINGS | 44 | UI_CURRENT_CONTAINER |
| 4 | UI_DIALOG | 45 | UI_CONTAINER_REWARDS |
| 5 | UI_RADIAL_MENU | 46 | UI_ACTION |
| 6 | UI_SNACKBAR | 47 | UI_MOBILE_PHONE |
| 7 | UI_ANIMATION_MENU | 48 | UI_GAS_STATION |
| 8 | UI_HUD | 49 | UI_CINEMA_EFFECT |
| 9 | UI_AUTHORIZATION | 50 | UI_NEW_QUEST |
| 15 | UI_QUEST_DIALOG | 51 | UI_VEHICLE_PLATE |
| 17 | UI_PERSONAL_PROPERTY | 52 | UI_INVENTORY |
| 21 | UI_METAL_DETECTOR | 53 | UI_INTERACTION_BUTTON |
| 25 | UI_UNIVERSAL_ACTION | 54 | UI_MAIN_MENU |
| 28 | UI_EXCAVATION_SCREEN | 55 | UI_STREET_FOOD |
| 29 | UI_QUESTS_SCREEN | 56 | UI_PLAYER_LIST |
| 33 | UI_DICE_SCREEN | 57 | UI_TRADE |
| 38 | UI_PASS_PROMO | 58 | UI_CRAFT |
| 39 | UI_USER_BATTLE_PASS | 59 | UI_WORKSHOP |
| 40 | UI_MAIN_BATTLE_PASS | 60 | UI_GLOVO |
| 41 | UI_NEWBIE_BATTLE_PASS | 61 | UI_LAVKA |
| 62 | UI_INPUT_LAYOUT | 80 | UI_SITUATION |
| 63 | UI_STREAM_VIDEO | 81 | UI_FISHING |
| 64 | UI_INVENTORY_SECURITY_SCREEN | 82 | UI_NEW_CONTAINER |
| 65 | UI_INVENTORY_WAREHOUSE | 83 | UI_BANNERS |
| 66 | UI_INVENTORY_VEHICLE_SCREEN | 84 | UI_BUSINESS |
| 67 | UI_INVENTORY_WALLET_SCREEN | 87 | UI_HINTS |
| 68 | UI_PRODUCTS_SCREEN | 88 | UI_DAILY_REWARD |
| 72 | UI_PREVIEW_USER_INVENTORY | 89 | UI_LINKING |
| 73 | UI_VIDEO_ADVICE | 91 | UI_CARS |
| 74 | UI_TUNING_SCREEN | 94 | UI_HOUSES |
| 75 | UI_EMPLOYMENT_TASK | 95 | UI_POTIONS |
| 76 | UI_CASE_ROULETTE | 96 | UI_CASINO |
| 77 | UI_FLASH_GRENADE | 97 | UI_REWARDS_SCREEN |
| 98 | UI_BLUEPRINT | 110 | UI_EASTER_MENU |
| 99 | UI_BATTLE_PASS | 111 | UI_BP_EVENT_CHOICE |
| 101 | UI_CATALOG | 112 | UI_CONVEYOR_GAME |
| 104 | UI_DOCUMENTS | | |

## API

### Работа со Svelte-интерфейсами

```pawn
// Показать интерфейс игроку
ShowPlayerSvelteApp(playerid, SVELTE_APP_DONATESHOP, true);

// Скрыть текущий интерфейс
HidePlayerSvelteApp(playerid, true);

// Разрешить закрытие интерфейса по ESC
Svelte:SetPlayerAllowedEsc(playerid, true);

// Получить текущий интерфейс
new app = GetPlayerSvelteAppId(playerid);
```

### Модальные окна

```pawn
// Показать модальное окно
ShowPlayerModal(playerid, MODAL_TYPE_DIALOG, true, data);

// Скрыть модальное окно
HidePlayerModal(playerid, MODAL_TYPE_DIALOG, true);

// Проверка открытия
if(IsModalTypeOpened(playerid, MODAL_TYPE_DIALOG)) { ... }
```

### Определение клиента

```pawn
if(IsLauncherConnected(playerid))    // Arizona PC
if(IsMobileConnected(playerid))      // Arizona Mobile
if(IsTrilogyConnected(playerid))     // Arizona Trilogy
```

### Отправка событий на фронтенд

```pawn
SendMessageToFront(playerid, "event.donateShop.updateProduct", json_data);
```

## Колбэки

```pawn
// Показ Svelte-интерфейса
public OnPlayerShowSvelteApp(playerid, app, index, const app_name[]);

// Скрытие Svelte-интерфейса
public OnPlayerHideSvelteApp(playerid, app, cursor);

// Сообщение от интерфейса
public OnPlayerSvelteAppMessage(playerid, index, app, const message[]);

// Нажатие ESC в интерфейсе
public OnPlayerClickEscSvelteApp(playerid, app, index);

// Модальные окна
public OnPlayerShowModal(playerid, modal);
public OnPlayerHideModal(playerid, modal);
```

## Команды (для тестирования)

| Команда | Описание |
|---------|----------|
| `/debug` | Отладочная информация о клиенте |
| `/donate` | Открыть донат-магазин |
| `/numbers` | Номера автомобилей |
| `/security` | Баннер безопасности |
| `/dialog [style]` | Тест кастомных диалогов |
| `/showdialogtip` | Показать подсказку |
| `/hidedialogtip` | Скрыть подсказку |