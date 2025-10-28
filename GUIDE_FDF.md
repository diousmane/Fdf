# 📚 Guide Complet FdF - Projet Final

## 🎯 Introduction

**FdF** (Fil de Fer) est un projet qui affiche une carte 3D en wireframe.

- Lecture d'un fichier `.fdf` avec des altitudes
- Projection isométrique 3D→2D
- Affichage de la carte avec des lignes blanches
- Gestion fenêtre et événements

---

## 📁 Structure du Projet FINALE

```
Fdf/
├── src/
│   ├── main.c         ← Point d'entrée, vérifie .fdf, lance programme
│   ├── parsing.c      ← Fonction principale parse_map() + free_map()
│   ├── parse_utils.c  ← count_lines(), count_columns(), fill_map()
│   ├── projection.c   ← abs_value(), project_point() isométrique
│   ├── drawing.c      ← put_pixel(), draw_map(), draw_connections()
│   ├── draw_line.c    ← line_horizontal(), line_vertical(), draw_line()
│   └── display.c      ← init_mlx(), calc_zoom(), key_press(), close_win()
├── includes/
│   └── fdf.h          ← Toutes les déclarations
├── libft/             ← Vos fonctions (ft_split, ft_atoi, get_next_line...)
├── minilibx-linux/    ← Bibliothèque graphique MLX
├── test_maps/         ← Fichiers .fdf de test
├── Makefile           ← Compilation
└── GUIDE_FDF.md       ← Ce fichier

```

---

## 🔍 PARTIE 1 : Concepts de Base

### Les Structures

**`t_map`** - Stocke les données de la carte :
```c
typedef struct s_map
{
    int **z_matrix;      // Tableau 2D des altitudes (Z) - Seul champ utilisé
    int width;           // Largeur de la carte
    int height;          // Hauteur de la carte
    int z_min;           // Altitude minimale
    int z_max;           // Altitude maximale
}   t_map;
```

**`t_window`** - Données de la fenêtre MLX :
```c
typedef struct s_window
{
    void *mlx;              // Instance MLX
    void *win;             // Fenêtre
    void *img;             // Image (buffer)
    char *img_data;        // Données pixels de l'image
    int bits_per_pixel;
    int line_length;
    int endian;
    t_map *map;            // Pointeur vers la carte
    int zoom;              // Zoom calculé automatiquement
}   t_window;
```

**`t_point`** - Coordonnées d'un point :
```c
typedef struct s_point
{
    int x;
    int y;
}   t_point;
```

### Syntaxe Pointer (`->`)

```c
t_window *win;        // Pointeur
win->mlx              // Accès avec FLÈCHE
win->map->z_matrix[0][0]  // Chaînage
```

---

## 📖 PARTIE 2 : Fonctionnement Étape par Étape

### Étape 0 : Le Fichier `.fdf`

Format :
```
0 0 0 0 0
0 5 5 0 0
0 0 0 0 0
```

- Chaque ligne = une ligne Y de la carte
- Chaque nombre = altitude Z à la position (X, Y)
- Exemple : ligne 1, colonne 2 → altitude 5

### Étape 1 : Main (`main.c`)

```c
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        ft_putstr_fd("Usage: ./fdf map_file.fdf\n", 2);
        return (1);
    }
    if (!check_extension(argv[1]))
    {
        ft_putstr_fd("Error: file must have .fdf extension\n", 2);
        return (1);
    }
    if (!launch_fdf(argv[1]))
    {
        ft_putstr_fd("Error\n", 2);
        return (1);
    }
    return (0);
}
```

**Fonctions appelées** :
- `launch_fdf()` → lance le programme
- `check_extension()` → vérifie `.fdf`

---

### Étape 2 : Parsing (`parsing.c` + `parse_utils.c`)

#### `count_lines()` - Compter les lignes

```c
int count_lines(char *filename)
{
    fd = open(filename, O_RDONLY);
    nb_lines = 0;
    line = get_next_line(fd);
    while (line)
    {
        nb_lines++;
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return nb_lines;
}
```

#### `count_columns()` - Compter les colonnes

```c
int count_columns(char *line)
{
    words = ft_split(line, ' ');
    nb = 0;
    while (words[nb])
        nb++;
    // Libère et retourne
    return nb;
}
```

#### `fill_map()` - Remplir la matrice

```c
void fill_map(char *filename, t_map *map)
{
    allocate_matrices(map);  // Crée les tableaux 2D
    
    fd = open(filename, O_RDONLY);
    y = 0;
    while (y < map->height)
    {
        line = get_next_line(fd);
        numbers = ft_split(line, ' ');
        
        x = 0;
        while (x < map->width)
        {
            map->z_matrix[y][x] = ft_atoi(numbers[x]);
            // Calcule min/max
            if (map->z_matrix[y][x] < map->z_min)
                map->z_min = map->z_matrix[y][x];
            if (map->z_matrix[y][x] > map->z_max)
                map->z_max = map->z_matrix[y][x];
            x++;
        }
        free(line);
        y++;
    }
    close(fd);
}
```

**Résultat** : 
```
z_matrix[y][x] = altitude à (x, y)
```

---

### Étape 3 : Projection (`projection.c`)

#### Projection Isométrique

```c
void project_point(t_point *pt, int z, t_window *win)
{
    old_x = pt->x * win->zoom;      // Applique zoom
    old_y = pt->y * win->zoom;
    
    // Formules isométriques
    pt->x = (old_x - old_y) * 0.866;    // cos(30°)
    pt->y = (old_x + old_y) * 0.5 - z * win->zoom * 0.5;  // sin(30°) + altitude
    
    // Centre l'écran
    pt->x += WIDTH / 2;
    pt->y += HEIGHT / 2;
}
```

**Formules** :
- `0.866 = cos(30°)` → aplatit horizontalement
- `0.5 = sin(30°)` → penche verticalement
- `- z * win->zoom * 0.5` → applique l'altitude

**Exemple** :
```
Point 3D: (5, 3, 10)
    ↓
Projection isométrique
    ↓
Point écran: (x_écran, y_écran)
```

---

### Étape 4 : Dessin (`drawing.c` + `draw_line.c`)

#### `put_pixel()` - Écrire un pixel

```c
void put_pixel(t_window *win, int x, int y, int color)
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        position = y * win->line_length + x * (win->bits_per_pixel / 8);
        *(unsigned int *)(win->img_data + position) = color;
    }
}
```

#### Algorithme de Bresenham

```c
void line_horizontal(t_window *win, t_point p1, t_point p2)
{
    dx = abs_value(p2.x - p1.x);  // Distance horizontale
    dy = abs_value(p2.y - p1.y);  // Distance verticale
    
    err = dx / 2;
    while (p1.x <= p2.x)
    {
        put_pixel(win, p1.x, p1.y, COLOR_WHITE);
        err -= dy;
        if (err < 0)
        {
            p1.y += dir_y;  // Monte ou descend
            err += dx;
        }
        p1.x++;
    }
}
```

**Principe** : Avance pixel par pixel et décide si monter/descendre.

#### `draw_map()` - Dessine toute la carte

```c
void draw_map(t_window *win)
{
    y = 0;
    while (y < win->map->height)
    {
        x = 0;
        while (x < win->map->width)
        {
            p1.x = x;
            p1.y = y;
            project_point(&p1, win->map->z_matrix[y][x], win);
            draw_connections(win, p1, x, y);
            x++;
        }
        y++;
    }
}
```

---

### Étape 5 : Affichage (`display.c`)

#### `init_mlx()` - Initialise la fenêtre

```c
void init_mlx(t_window *win)
{
    win->mlx = mlx_init();
    win->win = mlx_new_window(win->mlx, WIDTH, HEIGHT, TITLE);
    win->img = mlx_new_image(win->mlx, WIDTH, HEIGHT);
    win->img_data = mlx_get_data_addr(win->img, ...);
}
```

#### `calc_zoom()` - Calcule le zoom automatique

```c
void calc_zoom(t_window *win)
{
    max_dimension = win->map->width + win->map->height;
    zoom_x = WIDTH / max_dimension;
    zoom_y = HEIGHT / max_dimension;
    
    if (zoom_x < zoom_y)
        win->zoom = zoom_x;
    else
        win->zoom = zoom_y;
    
    win->zoom = (win->zoom * 80) / 100;  // 80% pour marge
    if (win->zoom < 1)
        win->zoom = 1;
}
```

**Pourquoi 80% ?**
- Pour laisser une marge et éviter que la carte soit coupée
- Garantit que toute la carte est visible

#### `key_press()` + `close_win()` - Gestion événements

```c
int key_press(int key, t_window *win)
{
    if (key == KEY_ESC)
        close_win(win);
    return (0);
}

int close_win(t_window *win)
{
    mlx_destroy_image(...);
    mlx_destroy_window(...);
    free_map(win->map);
    free(win);
    exit(0);
}
```

---

## 🔄 Flux Complet

```
1. main()
   ↓ appelle launch_fdf()
   
2. launch_fdf()
   ├─ parse_map()           ← Lit le fichier .fdf
   │   ├─ count_lines()
   │   ├─ count_columns()
   │   └─ fill_map()
   ├─ init_mlx()            ← Crée fenêtre MLX
   ├─ calc_zoom()           ← Calcule zoom à 80%
   ├─ draw_map()            ← Dessine la carte
   │   ├─ project_point()   ← Transforme 3D→2D
   │   └─ draw_line()       ← Trace ligne
   ├─ mlx_put_image_to_window()  ← Affiche l'image
   └─ mlx_loop()            ← Attend événements

3. Utilisateur appuie ESC
   ↓
4. key_press() → close_win() → exit()
```

---

## 💡 Points Importants

### Projection Isométrique

- Vue 3D à 45° depuis le coin
- Formules mathématiques standard : `cos(30°)` et `sin(30°)`
- Utilisée dans de nombreux jeux (Minecraft, etc.)

### Zoom Automatique

- Calculé selon `width + height` de la carte
- Réduit à 80% pour éviter les débordements
- S'adapte automatiquement aux différentes cartes

### Algorithme de Bresenham

- Trace des lignes droites pixel par pixel
- Gère les diagonales proprement
- Célèbre algorithm de dessin de ligne

---

## ✅ Caractéristiques du Projet FINAL

✅ Pas de couleurs (mandatory)  
✅ Projection isométrique standard  
✅ Zoom automatique à 80%  
✅ Gestion ESC et croix  
✅ Fenêtre 1920×1080  
✅ Fichiers organisés (7 fichiers .c)  
✅ Norminette OK  
✅ Pas de fonctions `static` inutiles  
✅ Protection fichier `.fdf`  

---

## 🚀 Utilisation

```bash
# Compiler
make

# Tester une carte
./fdf test_maps/42.fdf

# Nettoyer
make fclean
```

---

## 📝 Fonctions Libft Utilisées

| Fonction | Usage |
|----------|-------|
| `ft_split()` | Découpe ligne en nombres |
| `ft_atoi()` | Convertit "10" → 10 |
| `ft_strlen()` | Longueur chaîne (check_extension) |
| `get_next_line()` | Lit fichier ligne par ligne |
| `ft_putstr_fd()` | Messages d'erreur |

---

🎓 **Votre projet FdF est maintenant complet et prêt pour la correction !**

