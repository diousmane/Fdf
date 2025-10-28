# 📚 Guide Complet du Projet FdF

## 🎯 Introduction

**FdF** (Fil de Fer = Wireframe) est un projet qui dessine des cartes 3D en 2D sur votre écran.

Imaginez une **carte de montagne** vue d'en haut :
- Chaque point a une **altitude** (hauteur)
- On les connecte entre eux avec des **lignes**
- On transforme ça pour le voir en **3D** à l'écran

---

## 📁 Structure du Projet

```
Fdf/
├── src/              ← CODE SOURCE (votre travail)
│   ├── main.c        ← Point de départ du programme
│   ├── parsing.c     ← Lecture du fichier .fdf
│   ├── parse_utils.c ← Fonctions utilitaires pour lire
│   ├── projection.c  ← Transformation 3D→2D
│   ├── drawing.c     ← Dessin sur l'image
│   ├── draw_line.c   ← Traçage des lignes pixel par pixel
│   └── display.c     ← Création fenêtre + événements
├── includes/
│   └── fdf.h         ← Toutes les déclarations (comme un sommaire)
├── libft/            ← Vos fonctions de base
├── minilibx-linux/   ← Bibliothèque pour créer fenêtres
└── test_maps/        ← Fichiers de test .fdf
```

---

## 🔍 PARTIE 1 : Les Concepts de Base

### 1. Qu'est-ce qu'une Structure (`struct`) ?

Une structure, c'est une **boîte** qui regroupe plusieurs variables ensemble.

**Exemple dans la vie** :
- Une personne a : nom, âge, taille
- Au lieu de créer 3 variables séparées, on crée une structure "Personne"

**En code** :
```c
// Structure pour stocker une carte
typedef struct s_map
{
    int **z_matrix;      // Tableau 2D des altitudes
    int **color_matrix;  // Tableau 2D des couleurs
    int width;           // Largeur de la carte
    int height;          // Hauteur de la carte
    int z_min;           // Altitude minimale
    int z_max;           // Altitude maximale
}   t_map;
```

**Utilisation** :
```c
t_map carte;                // Crée une carte vide
carte.width = 10;           // Met largeur à 10
carte.z_matrix[0][0] = 5;   // Met altitude à (0,0) = 5
```

### 2. Qu'est-ce qu'un Pointeur (`*`) ?

Un pointeur, c'est une **adresse en mémoire**.

**Analogie** :
- Votre maison = variable normale
- L'adresse de votre maison sur un papier = pointeur

**En code** :
```c
t_map carte;         // Structure NORMALE (la maison)
t_map *carte;        // POINTEUR vers structure (adresse)
```

**Différence `.` vs `->`** :
```c
t_map carte;         // Pas de *
carte.width          // On utilise le POINT (.)

t_map *carte;        // Il y a un *
carte->width         // On utilise la FLÈCHE (->)
```

**Pourquoi des pointeurs ?**
- Pour **passer des structures** à des fonctions
- Pour **modifier** la structure dans une fonction
- Pour **économiser la mémoire**

---

## 📖 PARTIE 2 : Le Fonctionnement Étape par Étape

### Étape 0 : Le Fichier `.fdf`

Le fichier `.fdf` contient les altitudes de votre carte.

**Exemple** (`test_maps/42.fdf`) :
```
0 0 0
0 5 0
0 0 0
```

**Ce que ça signifie** :
- Ligne 1 : point à (0,0) altitude 0, point à (1,0) altitude 0, point à (2,0) altitude 0
- Ligne 2 : point à (0,1) altitude 0, point à (1,1) altitude 5 (SOMMET!), point à (2,1) altitude 0
- Ligne 3 : point à (0,2) altitude 0, point à (1,2) altitude 0, point à (2,2) altitude 0

C'est une **petite montagne** avec un sommet au milieu !

---

### Étape 1 : Le Programme Démarre (`main.c`)

**Fichier** : `src/main.c`

```c
int main(int argc, char **argv)
{
    // 1. Vérifie qu'on a donné un argument
    if (argc != 2)
    {
        ft_putstr_fd("Usage: ./fdf fichier.fdf\n", 2);
        return (1);
    }
    
    // 2. Lance le programme principal
    launch_fdf(argv[1]);
    
    return (0);
}
```

**Explication ligne par ligne** :

1. `int argc` = nombre d'arguments
   - `argc = 1` → juste le nom du programme
   - `argc = 2` → programme + fichier .fdf (✅ bon !)
   - `argc = 3` → trop d'arguments (❌ erreur)

2. `char **argv` = tableau de chaînes de caractères
   - `argv[0]` = "./fdf"
   - `argv[1]` = "test_maps/42.fdf"

3. Si pas le bon nombre d'arguments → message d'erreur et on quitte

4. Sinon → on appelle `launch_fdf()` avec le fichier

---

### Étape 2 : Parsing - Lecture du Fichier (`parsing.c` + `parse_utils.c`)

**Objectif** : Lire le fichier `.fdf` et remplir les tableaux

#### 2.1. Compter les Lignes (`count_lines()`)

```c
int count_lines(char *filename)
{
    int  nb_lines = 0;
    char *line;
    
    // Ouvre le fichier
    fd = open(filename, O_RDONLY);
    
    // Lit ligne par ligne
    line = get_next_line(fd);
    while (line)
    {
        nb_lines++;        // ← On compte
        free(line);        // ← On libère la mémoire
        line = get_next_line(fd);
    }
    
    close(fd);
    return nb_lines;       // ← Nombre de lignes
}
```

**Exemple** : Si le fichier a 3 lignes → retourne `3`

#### 2.2. Compter les Colonnes (`count_columns()`)

```c
int count_columns(char *line)
{
    // Exemple: line = "10 20 30"
    char **words = ft_split(line, ' ');
    // words = ["10", "20", "30"]
    
    int nb = 0;
    while (words[nb])
        nb++;
    // nb = 3
    
    // Libère la mémoire
    free(words);
    return nb;  // ← 3
}
```

**Astuce** : `ft_split()` découpe une chaîne par un caractère
- Entrée : `"10 20 30"` avec `' '`
- Sortie : `["10", "20", "30"]`

#### 2.3. Remplir les Tableaux (`fill_map()`)

```c
void fill_map(char *filename, t_map *map)
{
    // 1. Alloue de la mémoire pour les tableaux
    map->z_matrix = malloc(...);      // Tableau d'altitudes
    map->color_matrix = malloc(...);   // Tableau de couleurs
    
    // 2. Ouvre le fichier
    fd = open(filename, O_RDONLY);
    
    // 3. Lit chaque ligne
    y = 0;
    while (y < map->height)
    {
        line = get_next_line(fd);
        
        // 4. Découpe la ligne en nombres
        numbers = ft_split(line, ' ');
        
        // 5. Remplit chaque colonne
        x = 0;
        while (x < map->width)
        {
            // Convertit "10" en nombre 10
            map->z_matrix[y][x] = ft_atoi(numbers[x]);
            map->color_matrix[y][x] = COLOR_WHITE;
            
            // Met à jour min/max
            if (map->z_matrix[y][x] < map->z_min)
                map->z_min = map->z_matrix[y][x];
            if (map->z_matrix[y][x] > map->z_max)
                map->z_max = map->z_matrix[y][x];
            
            x++;
        }
        y++;
    }
}
```

**Résultat** :
- `map->z_matrix[0][0] = 0`
- `map->z_matrix[0][1] = 0`
- `map->z_matrix[1][1] = 5`  ← Le sommet !
- etc.

---

### Étape 3 : Projection - Transformation 3D→2D (`projection.c`)

**Objectif** : Transformer les coordonnées de la carte en coordonnées écran

#### 3.1. Valeur Absolue (`abs_value()`)

```c
int abs_value(int n)
{
    if (n < 0)
        return (-n);  // Exemple: -5 → 5
    return (n);       // Exemple: 5 → 5
}
```

**Utilité** : Pour calculer les distances (toujours positives)

#### 3.2. Projection Isométrique (`project_point()`)

C'est le **truc magique** qui transforme en 3D !

```c
void project_point(t_point *pt, int z, t_window *win)
{
    int old_x = pt->x * win->zoom;      // Applique le zoom
    int old_y = pt->y * win->zoom;
    
    // Formules de projection isométrique
    pt->x = (old_x - old_y) * 0.866;    // 0.866 = cos(30°)
    pt->y = (old_x + old_y) * 0.5 - z;  // 0.5 = sin(30°)
    
    // Centre au milieu de l'écran
    pt->x += WIDTH / 2;
    pt->y += HEIGHT / 2;
}
```

**Explication des formules** :

1. **`(old_x - old_y) * 0.866`** :
   - C'est comme "regarder en diagonale"
   - Multiplie par 0.866 pour "aplatir"

2. **`(old_x + old_y) * 0.5 - z`** :
   - Combinaison de x et y
   - Soustrait z (altitude) pour la profondeur

3. **`+= WIDTH / 2` et `+= HEIGHT / 2`** :
   - Déplace tout au centre de l'écran
   - Sinon ce serait dans un coin

**Résultat** : Vous obtenez des coordonnées (x, y) pour l'écran !

---

### Étape 4 : Dessin - Tracer les Lignes (`drawing.c` + `draw_line.c`)

**Objectif** : Dessiner la carte pixel par pixel

#### 4.1. Mettre un Pixel (`put_pixel()`)

```c
void put_pixel(t_window *win, int x, int y, int color)
{
    // Vérifie que le pixel est dans la fenêtre
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        // Calcule où se trouve le pixel en mémoire
        int position = y * win->line_length + x * (win->bits_per_pixel / 8);
        
        // Écrit la couleur directement en mémoire
        *(unsigned int *)(win->img_data + position) = color;
    }
}
```

**Explication** :
- `line_length` = largeur d'une ligne en octets
- `y * line_length` = saute y lignes
- `x * (bits_per_pixel / 8)` = avance de x pixels
- `*(unsigned int *)` = convertit en nombre (couleur)

#### 4.2. Algorithme de Bresenham (`draw_line.c`)

C'est un algorithme célèbre pour tracer des lignes !

**Problème** : Comment tracer une ligne droite entre 2 points ?
- On ne peut dessiner que des pixels (pas de demi-pixels)

**Solution de Bresenham** :
- On avance pixel par pixel
- On décide à chaque fois : monter/descendre ou non
- Exemple : tracer ligne de (0,0) à (10,3)

```c
void draw_line(t_window *win, t_point p1, t_point p2)
{
    // Calcule les différences
    dx = |p2.x - p1.x|;  // Distance horizontale
    dy = |p2.y - p1.y|;  // Distance verticale
    
    // Si ligne plutôt horizontale
    if (dx >= dy)
    {
        // Tracer horizontalement
        while (p1.x <= p2.x)
        {
            put_pixel(win, p1.x, p1.y, COLOR_WHITE);
            
            // Calcule si on doit monter
            err -= dy;
            if (err < 0)
            {
                p1.y++;      // On monte !
                err += dx;   // Ajuste
            }
            p1.x++;  // On avance toujours
        }
    }
    else
    {
        // Tracer verticalement (même principe mais en Y)
    }
}
```

**Image** :
```
Point 1                Point 2
   *                      *
    \                    /
     \                  /
      \                /
       \              /
        *─────*─────*     
```

#### 4.3. Dessiner Toute la Carte (`draw_map()`)

```c
void draw_map(t_window *win)
{
    y = 0;
    while (y < win->map->height)
    {
        x = 0;
        while (x < win->map->width)
        {
            // 1. Prend un point de la carte
            p1.x = x;
            p1.y = y;
            
            // 2. Le projette en 2D
            project_point(&p1, win->map->z_matrix[y][x], win);
            
            // 3. Dessine les connexions vers droite et bas
            if (x < win->map->width - 1)
                // Dessine vers la droite
                draw_line(win, p1, point_droite);
            
            if (y < win->map->height - 1)
                // Dessine vers le bas
                draw_line(win, p1, point_bas);
            
            x++;
        }
        y++;
    }
}
```

**Résultat** : Tous les points sont connectés !

---

### Étape 5 : Affichage - Création Fenêtre (`display.c`)

**Objectif** : Créer une fenêtre et gérer les événements

#### 5.1. Initialiser MLX (`init_mlx()`)

```c
void init_mlx(t_window *win)
{
    // 1. Initialise la bibliothèque MLX
    win->mlx = mlx_init();
    
    // 2. Crée une fenêtre 1920x1080
    win->win = mlx_new_window(win->mlx, WIDTH, HEIGHT, TITLE);
    
    // 3. Crée une image (buffer) pour dessiner
    win->img = mlx_new_image(win->mlx, WIDTH, HEIGHT);
    
    // 4. Récupère l'adresse mémoire de l'image
    win->img_data = mlx_get_data_addr(win->img, ...);
}
```

**Pourquoi une image ?**
- On dessine D'ABORD dans l'image (en mémoire)
- Puis on AFFICHE l'image en une fois
- Plus rapide que de dessiner pixel par pixel à l'écran

#### 5.2. Calculer le Zoom (`calc_zoom()`)

```c
void calc_zoom(t_window *win)
{
    // Calcule zoom pour que la carte tienne dans 80% de l'écran
    zoom_x = (WIDTH * 0.8) / map->width;
    zoom_y = (HEIGHT * 0.8) / map->height;
    
    // Prend le plus petit (pour que tout tienne)
    if (zoom_x < zoom_y)
        win->zoom = zoom_x;
    else
        win->zoom = zoom_y;
}
```

**Exemple** :
- Fenêtre : 1920×1080
- Carte : 100×50
- `zoom_x = 1920*0.8 / 100 = 15.36`
- `zoom_y = 1080*0.8 / 50 = 17.28`
- → Prend `15` (le plus petit)

#### 5.3. Gérer les Événements (`key_press()` + `close_win()`)

```c
int key_press(int key, t_window *win)
{
    if (key == KEY_ESC)      // Si on appuie sur ESC
        close_win(win);      // Ferme le programme
    return (0);
}

int close_win(t_window *win)
{
    // Libère TOUTE la mémoire
    mlx_destroy_image(...);
    mlx_destroy_window(...);
    free_map(win->map);      // Libère la carte
    free(win);               // Libère la fenêtre
    exit(0);                 // Quitte
}
```

---

## 🔄 FLUX COMPLET DU PROGRAMME

```
1. main() 
   ↓
2. launch_fdf()
   ├─ parse_map()        ← Lit le fichier .fdf
   │   ├─ count_lines()
   │   ├─ count_columns()
   │   └─ fill_map()
   ├─ init_mlx()         ← Crée fenêtre
   ├─ calc_zoom()        ← Calcule zoom
   ├─ draw_map()         ← Dessine la carte
   │   ├─ project_point()    (3D→2D)
   │   └─ draw_line()         (tracer lignes)
   ├─ mlx_put_image_to_window()  ← Affiche
   └─ mlx_loop()          ← Attend événements
   
3. L'utilisateur appuie ESC
   ↓
4. key_press() appelle close_win()
   ↓
5. close_win() libère tout et exit()
```

---

## 💡 Concepts Importants à Retenir

### 1. Les Tableaux 2D (`int **`)

```c
int **z_matrix;   // ← Tableau de tableaux

// Création :
z_matrix = malloc(sizeof(int *) * height);  // Alloue les lignes
z_matrix[0] = malloc(sizeof(int) * width);  // Alloue ligne 0
z_matrix[1] = malloc(sizeof(int) * width);  // Alloue ligne 1

// Utilisation :
z_matrix[y][x] = 10;  // Met 10 à la position (x,y)
```

**C'est comme un tableau Excel** :
```
      x=0  x=1  x=2
y=0    0    0    0
y=1    0    5    0
y=2    0    0    0
```

### 2. Le Zoom

Le zoom, c'est un **multiplicateur** :
- `zoom = 1` → taille normale
- `zoom = 10` → 10 fois plus grand
- `zoom = 0.5` → 2 fois plus petit

**Dans le code** :
```c
pt->x = x * win->zoom;  // Multiplie par le zoom
```

### 3. La Projection Isométrique

C'est une **vue en 3D** à 45° depuis le coin.

**Sans projection** (vu de dessus) :
```
0 0 0
0 5 0
0 0 0
```

**Avec projection isométrique** (vue 3D) :
```
   0
  / \
 5   0
```

**Les formules** :
- `x_2D = (x - y) * cos(30°)`  → On "tourne"
- `y_2D = (x + y) * sin(30°) - z`  → On "penche" + altitude

---

## 🎨 COMMENT ÇA RESSEMBLE ?

### Sans Projet :
```
Fichier : "0 5 0"
```

### Après Parsing :
```
Tableau z_matrix[3][3] :
0 0 0
0 5 0
0 0 0
```

### Après Projection Isométrique :
```
     *  ← point (0,1) à altitude 5
    /|\
   / | \
  /  |  \
 *---*---*
```

### Affiché à l'Écran :
Une **petite montagne 3D** en blanc sur fond noir !

---

## 🛠️ Les Fonctions de Libft Utilisées

| Fonction | Utilité | Exemple |
|----------|---------|---------|
| `ft_split(str, ' ')` | Découpe par espaces | `"10 20"` → `["10", "20"]` |
| `ft_atoi(str)` | Convertit en nombre | `"42"` → `42` |
| `get_next_line(fd)` | Lit une ligne | Lit fichier ligne par ligne |
| `ft_putstr_fd(s, 2)` | Affiche erreur | `"Error\n"` sur stderr |
| `malloc()` / `free()` | Gère mémoire | Alloue/libère |

---

## 🚀 Pour Tester

```bash
# Compiler
make

# Lancer une carte
./fdf test_maps/42.fdf

# Appuyer sur ESC pour quitter
```

---

## ✅ Récapitulatif

1. **main.c** : Point d'entrée, vérifie arguments
2. **parsing.c** : Lit fichier → remplit tableaux
3. **projection.c** : Transforme coordonnées 3D→2D
4. **drawing.c** : Dessine pixels dans l'image
5. **draw_line.c** : Trace des lignes (algorithme Bresenham)
6. **display.c** : Crée fenêtre, gère événements

**Le tout utilise vos fonctions libft** (`ft_split`, `ft_atoi`, `get_next_line`) !

---

🎓 **Maintenant vous comprenez comment fonctionne FdF !** 💪

