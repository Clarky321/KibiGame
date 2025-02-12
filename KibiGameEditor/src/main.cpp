#include <raylib.h>
#include <math.h>
#include <vector>
#include <raymath.h>

// Структура для персонажа
struct Character
{
    Vector2 position;      // Позиция в декартовых координатах
    Texture2D texture;     // Текстура персонажа
    Vector2 targetPos;     // Целевая позиция для движения
    bool isMoving;         // Флаг движения
    float moveSpeed;       // Скорость движения
    std::vector<Vector2> path;
};

// Функция преобразования декартовых координат в изометрические
Vector2 cartToIso(Vector2 cartPos, float tileSize)
{
    return {
        (cartPos.x - cartPos.y) * (tileSize / 2),
        (cartPos.x + cartPos.y) * (tileSize / 4)
    };
}

Vector2 screenToCart(Vector2 screenPos, float tileSize)
{
    Vector2 cartPos;
    cartPos.x = (screenPos.x / (tileSize / 2) + screenPos.y / (tileSize / 4)) / 2;
    cartPos.y = (screenPos.y / (tileSize / 4) - (screenPos.x / (tileSize / 2))) / 2;
    return cartPos;
}

void FindPath(Character& character, Vector2 target, int gridSize)
{
    character.path.clear();
    Vector2 current = character.position;

    // Простой алгоритм перемещения по осям
    while (current.x != target.x || current.y != target.y)
    {
        if (current.x < target.x) current.x++;
        else if (current.x > target.x) current.x--;
        else if (current.y < target.y) current.y++;
        else if (current.y > target.y) current.y--;

        if (current.x >= 0 && current.x < gridSize &&
            current.y >= 0 && current.y < gridSize)
        {
            character.path.push_back(current);
        }
        else break;
    }
}

void DrawIsoTileOutline(Vector2 isoPosition, float tileSize, Color color)
{
    Vector2 p1 = { isoPosition.x, isoPosition.y + tileSize / 4 };
    Vector2 p2 = { isoPosition.x + tileSize / 2, isoPosition.y };
    Vector2 p3 = { isoPosition.x + tileSize, isoPosition.y + tileSize / 4 };
    Vector2 p4 = { isoPosition.x + tileSize / 2, isoPosition.y + tileSize / 2 };

    DrawLineV(p1, p2, color);
    DrawLineV(p2, p3, color);
    DrawLineV(p3, p4, color);
    DrawLineV(p4, p1, color);
}

int main()
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    const int gridSize = 20;
    const float tileSize = 32.0f;

    InitWindow(screenWidth, screenHeight, "KibiGame");
    SetTargetFPS(60);

    // Загрузка текстуры тайла
    //Image tileImage = LoadImage("../../assets/tilemap/isometric_2/Blocks/blocks_16.png");
    Image tileImage = LoadImage("../../assets/tilemap/images/tile_000.png");
    Texture2D tileTexture = LoadTextureFromImage(tileImage);
    UnloadImage(tileImage);

    // Загрузка текстуры волка
    Image wolfImage = LoadImage("../../assets/character/wolf/img/wolf_idle_001.png");
    Texture2D wolfTexture = LoadTextureFromImage(wolfImage);
    UnloadImage(wolfImage);

    // Инициализация персонажа
    Character wolf = { {10, 10}, wolfTexture, {10, 10}, false, 5.0f, {} };

    Camera2D camera = { 0 };
    camera.zoom = 5.0f;
    camera.offset = Vector2 { screenWidth / 2, screenHeight / 2 };

    Vector2 hoverTile = { -1, -1 };

    while (!WindowShouldClose())
    {
        // Обновление позиции мыши
        Vector2 mouseScreenPos = GetMousePosition();
        Vector2 mouseWorldPos = GetScreenToWorld2D(mouseScreenPos, camera);
        Vector2 cartMouse = screenToCart(mouseWorldPos, tileSize);

        // Определение клетки под курсором
        hoverTile = { -1, -1 };
        if (cartMouse.x >= 0 && cartMouse.x < gridSize &&
            cartMouse.y >= 0 && cartMouse.y < gridSize)
        {
            hoverTile = { floorf(cartMouse.x), floorf(cartMouse.y) };
        }

        // Обработка клика правой кнопкой
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
            hoverTile.x != -1 && hoverTile.y != -1 &&
            !wolf.isMoving)
        {
            FindPath(wolf, hoverTile, gridSize);
            if (!wolf.path.empty())
            {
                wolf.targetPos = wolf.path[0];
                wolf.path.erase(wolf.path.begin());
                wolf.isMoving = true;
            }
        }

        // Обновление камеры
        camera.target = cartToIso(wolf.position, tileSize);
        camera.target.x += tileSize / 2;
        camera.target.y += tileSize / 4;

        // Обработка движения по WASD
        if (!wolf.isMoving)
        {
            Vector2 nextPos = wolf.position;
            if (IsKeyPressed(KEY_W)) nextPos.y -= 1; // Вперед
            if (IsKeyPressed(KEY_S)) nextPos.y += 1; // Назад
            if (IsKeyPressed(KEY_A)) nextPos.x -= 1; // Влево
            if (IsKeyPressed(KEY_D)) nextPos.x += 1; // Вправо

            // Проверяем, чтобы персонаж оставался в границах сетки
            if (nextPos.x >= 0 && nextPos.x < gridSize &&
                nextPos.y >= 0 && nextPos.y < gridSize)
            {
                wolf.targetPos = nextPos;
                wolf.isMoving = true;
            }
        }

        // Движение персонажа
        if (wolf.isMoving)
        {
            Vector2 direction = Vector2Subtract(wolf.targetPos, wolf.position);
            float distance = Vector2Length(direction);

            if (distance > 0.1f)
            {
                direction = Vector2Scale(Vector2Normalize(direction), wolf.moveSpeed * GetFrameTime());
                wolf.position = Vector2Add(wolf.position, direction);
            }
            else
            {
                wolf.position = wolf.targetPos;
                if (!wolf.path.empty())
                {
                    wolf.targetPos = wolf.path[0];
                    wolf.path.erase(wolf.path.begin());
                }
                else
                {
                    wolf.isMoving = false;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);

        // Отрисовка сетки
        for (int x = 0; x < gridSize; x++)
        {
            for (int y = 0; y < gridSize; y++)
            {
                Vector2 isoPos = cartToIso({ (float)x, (float)y }, tileSize);
                DrawTexture(tileTexture, isoPos.x, isoPos.y, WHITE);
            }
        }

        // Отрисовка выделения
        /*
                if (hoverTile.x != -1 && hoverTile.y != -1)
        {
            Vector2 isoPos = cartToIso(hoverTile, tileSize);
            DrawRectangleLinesEx(
                Rectangle {
                isoPos.x, isoPos.y, tileSize, tileSize / 2
            },
                2.0f,
                RED
            );
        }
        */

        // Отрисовка выделенной клетки
        if (hoverTile.x != -1 && hoverTile.y != -1)
        {
            Vector2 isoPos = cartToIso(hoverTile, tileSize);
            DrawIsoTileOutline(isoPos, tileSize, BLACK);
        }

        // Отрисовка волка
        Vector2 wolfIsoPos = cartToIso(wolf.position, tileSize);
        DrawTexture(wolf.texture, wolfIsoPos.x, wolfIsoPos.y, WHITE);

        EndMode2D();
        EndDrawing();
    }

    UnloadTexture(tileTexture);
    UnloadTexture(wolfTexture);
    CloseWindow();
    return 0;
}