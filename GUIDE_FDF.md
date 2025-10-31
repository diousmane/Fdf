# 📚 Guide Complet FdF - De Zéro à Maîtrise

## 🎯 Qu'est-ce que FdF ?

**FdF** (Fil de Fer) est un projet 42 qui affiche une carte topographique en **wireframe 3D**.

### Objectif du projet

- Lire un fichier `.fdf` contenant des **altitudes** (valeurs numériques)
- Transformer ces données 2D (x, y) avec altitudes (z) en une **projection isométrique**
- Afficher la carte avec des **lignes blanches** reliant les points
- Créer une fenêtre graphique et gérer les interactions (clavier, fermeture)

### Format du fichier `.fdf`

```
0 0 0 0 0
0 5 5 0 0
0 0 0 0 0
```

- **Chaque ligne** = une ligne Y de la carte
- **Chaque nombre** = altitude Z à la position (X, Y)
- **Exemple** : ligne 1 (y=1), colonne 2 (x=2) → altitude = 5

**Format des valeurs acceptées :**
- **Nombres décimaux** : `123`, `-45`, `+10`, `0`
- **Nombres hexadécimaux** : `0xFF`, `0xff`, `0xABC`, `0x10`, `-0xFF`
- **Format avec virgule (accepté, couleur ignorée)** : Les valeurs avec virgule comme `123,0xff00` sont acceptées, mais seule la partie avant la virgule est utilisée (pas de support des couleurs dans la partie obligatoire)

**Exemple avec hexadécimal :**
```
0 -5 10 0xFF -20
3 -8 0x10 5 -25
-2 7 0xABC -10 0x20
```

---

## 📁 Structure du Projet

```
Fdf/
├── src/
│   ├── main.c         ← Point d'entrée du programme
│   ├── parsing.c      ← Parsing principal + libération mémoire
│   ├── parse_utils.c  ← Helpers pour le parsing (count, allocate, parse value)
│   ├── parse_utils2.c ← Validation des nombres (décimal et hexadécimal)
│   ├── projection.c   ← Transformation 3D → 2D (isométrique)
│   ├── drawing.c      ← Dessin de la carte et connexions
│   ├── draw_line.c    ← Algorithme de Bresenham pour les lignes
│   └── display.c      ← Initialisation MLX, zoom, événements
├── includes/
│   └── fdf.h          ← Toutes les déclarations et structures
├── libft/             ← Bibliothèque personnelle (ft_split, ft_atoi, ft_atoi_hex, get_next_line...)
├── minilibx-linux/    ← Bibliothèque graphique MLX
├── test_maps/         ← Fichiers .fdf de test
├── Makefile           ← Compilation
└── GUIDE_FDF.md       ← Ce guide
```

---

## 🧩 Les Structures de Données

### 1. `t_map` - Les Données de la Carte

```c
typedef struct s_map
{
	int		**z_matrix;    // Tableau 2D des altitudes
	int		width;         // Largeur de la carte (nombre de colonnes)
	int		height;        // Hauteur de la carte (nombre de lignes)
}	t_map;
```

**Pourquoi cette structure ?**

- **`z_matrix`** : source de vérité des altitudes. Toutes les étapes (zoom, projection, dessin) la consultent.
- **`width`, `height`** : nécessaires pour itérer proprement et calculer le zoom automatique proportionnel à la taille de la carte.

**Stockage en mémoire :**

```
z_matrix[y][x] = altitude à la position (x, y)

Exemple pour une carte 3×2 :
z_matrix[0][0] = altitude à (x=0, y=0)
z_matrix[0][1] = altitude à (x=1, y=0)
z_matrix[1][0] = altitude à (x=0, y=1)
...
```

### 2. `t_window` - Chaque Chose du Contexte Graphique

```c
typedef struct s_window
{
	void		*mlx;              // Instance MLX (connexion X11)
	void		*win;              // Fenêtre graphique
	void		*img;              // Image (buffer mémoire)
	char		*img_data;         // Données pixels de l'image (adresse mémoire brute)
	int			bits_per_pixel;    // Nombre de bits par pixel (généralement 32)
	int			line_length;       // Taille d'une ligne en octets
	int			endian;            // Ordre des octets (little/big endian)
	t_map		*map;              // Pointeur vers la carte
	int			zoom;              // Facteur de zoom calculé automatiquement
}	t_window;
```

**Pourquoi chaque champ ?**

- **`mlx`, `win`** : descripteurs MinilibX indispensables pour créer/afficher une fenêtre X11 et recevoir des événements.
- **`img`, `img_data`, `bits_per_pixel`, `line_length`, `endian`** : tampon d'image en mémoire et métadonnées permettant d'écrire des pixels efficacement via `put_pixel` sans appeler `mlx_pixel_put` en boucle (trop lent).
- **`map`** : lien vers le modèle pour éviter les variables globales et permettre aux callbacks MLX (`key_press`, etc.) d'accéder au modèle.
- **`zoom`** : état calculé une fois selon la taille de la carte et la fenêtre, réutilisé par la projection pour garantir que la carte rentre à l'écran.

### 3. `t_point` - Coordonnées Écran

```c
typedef struct s_point
{
	int		x;  // Coordonnée X sur l'écran (en pixels)
	int		y;  // Coordonnée Y sur l'écran (en pixels)
}	t_point;
```

**Pourquoi cette structure ?**

- Représente un point 2D après projection. Séparer le point écran des données d'entrée (`z_matrix`) évite de modifier l'entrée et clarifie les conversions.
- Permet de passer facilement des points entre fonctions (projection, dessin de lignes).

---

## 🔑 Pointeurs vs Variables : `.` vs `->`

### Concept fondamental

- **Variable structure** : `t_point p1;` → on utilise `.` pour accéder aux champs : `p1.x = 5;`
- **Pointeur vers structure** : `t_point *p1;` → on utilise `->` pour accéder aux champs : `p1->x = 5;`

### Exemples dans le code

#### Cas 1 : Variable locale (`.`)

```c
void	draw_map(t_window *win)
{
	t_point	p1;  // Variable locale (pas un pointeur)

	p1.x = x;      // ✅ Correct : p1 est une variable, on utilise .
	p1.y = y;      // ✅ Correct
	project_point(&p1, ...);  // On passe l'adresse avec &
}
```

#### Cas 2 : Pointeur (`->`)

```c
void	project_point(t_point *pt, int z, t_window *win)
{
	pt->x = pt->x * win->zoom;  // ✅ Correct : pt est un pointeur, on utilise ->
	pt->y = pt->y * win->zoom;  // ✅ Correct
}
```

#### Cas 3 : Mélange (variable locale dont on passe l'adresse)

```c
void	draw_connections(t_window *win, t_point *p1, int x, int y)
{
	t_point	p2;  // Variable locale

	p2.x = x + 1;              // ✅ Correct : p2 est une variable, on utilise .
	p2.y = y;                  // ✅ Correct
	project_point(&p2, ...);   // On passe l'adresse pour que project_point utilise ->
	draw_line(win, p1, &p2);   // p1 est déjà un pointeur, &p2 donne un pointeur
}
```

### Règle à retenir

- **Variable** (`t_point p;`) → `.` (point)
- **Pointeur** (`t_point *p;`) → `->` (flèche)
- **Passage par adresse** : `&variable` transforme une variable en pointeur

---

## 📖 Étapes du Programme - Fonction par Fonction

### ÉTAPE 1 : Main (`main.c`)

#### `main(int argc, char **argv)`

**Rôle** : Point d'entrée du programme, validation des arguments.

**Logique** :

```c
int	main(int argc, char **argv)
{
	// 1. Vérifier qu'on a exactement 1 argument (le fichier .fdf)
	if (argc != 2)
	{
		ft_putstr_fd("Usage: ./fdf map_file.fdf\n", 2);
		return (1);
	}
	
	// 2. Vérifier que le fichier a bien l'extension .fdf
	if (!check_extension(argv[1]))
	{
		ft_putstr_fd("Error: file must have .fdf extension\n", 2);
		return (1);
	}
	
	// 3. Lancer le programme principal
	if (!start_fdf(argv[1]))
	{
		return (1);
	}
	
	return (0);  // Succès
}
```

#### `check_extension(char *filename)`

**Rôle** : Vérifier que le nom de fichier se termine par `.fdf`.

**Algorithme** :

```c
int	check_extension(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 4)  // Impossible d'avoir ".fdf" si moins de 4 caractères
		return (0);
	
	// Compare les 4 derniers caractères avec ".fdf"
	return (ft_strncmp(filename + len - 4, ".fdf", 4) == 0);
}
```

**Logique** :
1. Calcule la longueur du nom de fichier
2. Vérifie qu'il y a au moins 4 caractères
3. Compare les 4 derniers caractères avec ".fdf" en utilisant `ft_strncmp`

#### `start_fdf(char *filename)`

**Rôle** : Alloue les structures, parse le fichier, initialise et lance l'affichage.

**Logique** :

```c
int	start_fdf(char *filename)
{
	t_window	*win;
	t_map		*map;

	// 1. Allouer la mémoire pour les structures
	map = malloc(sizeof(t_map));
	win = malloc(sizeof(t_window));
	
	// 2. Vérifier que l'allocation a réussi
	if (!map || !win)
	{
		// Libérer ce qui a été alloué en cas d'échec
		if (map)
			free(map);
		if (win)
			free(win);
		return (0);
	}
	
	// 3. Parser le fichier .fdf et remplir la structure map
	if (!parse_map(filename, map))
	{
		free(map);
		free(win);
		return (0);
	}
	
	// 4. Lier la carte à la fenêtre
	win->map = map;
	
	// 5. Initialiser MLX, calculer le zoom, dessiner, et entrer dans la boucle
	return (init_and_run(win));
}
```

#### `init_and_run(t_window *win)`

**Rôle** : Orchestre l'initialisation graphique et la boucle d'événements.

**Logique** :

```c
int	init_and_run(t_window *win)
{
	// 1. Initialiser MLX (fenêtre, image, buffer)
	init_mlx(win);
	
	// 2. Calculer le zoom automatique pour que la carte rentre à l'écran
	calc_zoom(win);
	
	// 3. Dessiner toute la carte dans le buffer image
	draw_map(win);
	
	// 4. Afficher l'image dans la fenêtre
	mlx_put_image_to_window(win->mlx, win->win, win->img, 0, 0);
	
	// 5. Enregistrer les callbacks pour les événements
	mlx_hook(win->win, 2, 1L << 0, key_press, win);      // Touches clavier
	mlx_hook(win->win, 17, 0, close_win, win);            // Fermeture fenêtre
	
	// 6. Entrer dans la boucle d'événements (le programme reste vivant ici)
	mlx_loop(win->mlx);
	
	return (1);
}
```

---

### ÉTAPE 2 : Parsing (`parsing.c` + `parse_utils.c` + `parse_utils2.c`)

**Objectif** : Lire le fichier `.fdf` et remplir la matrice `z_matrix` avec les altitudes.

**Stratégie** : Parsing en **deux passes** avec validation de la forme rectangulaire.

#### `parse_map(char *filename, t_map *map)` - Fonction principale

**Rôle** : Coordonne tout le parsing.

**Logique** :

```c
int	parse_map(char *filename, t_map *map)
{
	int		fd;
	int		ok;

	// PASSE 1 : Obtenir les dimensions et valider la forme rectangulaire
	if (!get_map_dimensions(filename, map))
	{
		ft_putstr_fd("Error: invalid map\n", 2);
		return (0);
	}
	
	// PASSE 2 : Allouer la matrice
	if (!allocate_matrices(map))
		return (0);
	
	// PASSE 3 : Ouvrir le fichier et remplir la matrice
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		clear_zmatrix(map);
		return (0);
	}
	
	// PASSE 4 : Lire toutes les lignes et remplir la matrice
	ok = read_and_fill_rows(fd, map);
	close(fd);
	
	if (!ok)
	{
		ft_putstr_fd("Error: invalid map\n", 2);
		clear_zmatrix(map);
		return (0);
	}
	
	return (1);
}
```

**Pourquoi plusieurs passes ?**

1. **Validation précoce** : On vérifie que toutes les lignes ont le même nombre de colonnes avant d'allouer
2. **Norme 42** : Fonctions courtes (< 25 lignes), facilement testables
3. **Robustesse** : On détecte les erreurs tôt (fichier vide, largeur incohérente)
4. **Pas de realloc complexe** : On connaît la taille exacte avant d'allouer

#### `get_map_dimensions(char *filename, t_map *map)` - Obtenir les dimensions et valider

**Rôle** : Parcourir le fichier pour compter les lignes, obtenir la largeur et valider que toutes les lignes ont la même largeur.

**Algorithme** :

```c
int	get_map_dimensions(char *filename, t_map *map)
{
	int		fd;
	char	*line;
	int		first_width;

	// 1. Ouvrir le fichier
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	
	// 2. Initialiser les compteurs
	map->height = 0;
	first_width = 0;
	
	// 3. Lire ligne par ligne
	line = get_next_line(fd);
	while (line)
	{
		map->height++;
		
		// 4. Pour la première ligne, compter les colonnes (largeur)
		if (map->height == 1)
			first_width = count_columns(line);
		// 5. Pour les autres lignes, vérifier que la largeur correspond
		else if (!check_line_width(line, first_width))
			return (free(line), close(fd), 0);
		
		free(line);
		line = get_next_line(fd);
	}
	
	close(fd);
	
	// 6. Vérifier qu'on a au moins une ligne et une colonne
	map->width = first_width;
	return (map->height > 0 && map->width > 0);
}
```

#### `check_line_width(char *line, int expected_width)`

**Rôle** : Vérifier qu'une ligne a exactement le nombre de colonnes attendu.

**Logique** :

```c
int	check_line_width(char *line, int expected_width)
{
	int	current_width;

	current_width = count_columns(line);
	return (current_width == expected_width);
}
```

#### `count_columns(char *line)` - Compter les nombres sur une ligne

**Rôle** : Compter le nombre de nombres (séparés par des espaces) sur une ligne.

**Algorithme** :

```c
int	count_columns(char *line)
{
	int		count;
	int		in_word;
	int		i;

	if (!line)
		return (0);
	count = 0;
	in_word = 0;
	i = 0;
	while (line[i] && line[i] != '\n')
	{
		// Si on rencontre un caractère non-espace
		if (line[i] != ' ' && line[i] != '\t')
		{
			// Si on n'était pas déjà dans un mot, on commence un nouveau mot
			if (!in_word)
			{
				count++;      // Nouveau nombre trouvé
				in_word = 1;   // On est maintenant dans un mot
			}
		}
		else
			in_word = 0;  // On est sorti du mot
		i++;
	}
	return (count);
}
```

**Exemple** :
- Ligne : `"0 5 0xFF 5 0"`
- Comptage : `count = 5` (5 nombres séparés par des espaces)

#### `allocate_matrices(t_map *map)` - Allouer la matrice 2D

**Rôle** : Créer un tableau 2D de taille `height × width`.

**Algorithme** :

```c
int	allocate_matrices(t_map *map)
{
	int	i;

	// 1. Allouer un tableau de pointeurs (une ligne = un pointeur)
	map->z_matrix = malloc(sizeof(int *) * map->height);
	if (!map->z_matrix)
		return (0);
	
	// 2. Pour chaque ligne, allouer un tableau d'entiers
	i = 0;
	while (i < map->height)
	{
		map->z_matrix[i] = malloc(sizeof(int) * map->width);
		if (!map->z_matrix[i])
		{
			// En cas d'échec, libérer ce qui a été alloué
			while (--i >= 0)
				free(map->z_matrix[i]);
			free(map->z_matrix);
			map->z_matrix = NULL;
			return (0);
		}
		i++;
	}
	return (1);
}
```

**Structure en mémoire** :

```
z_matrix → [ptr0] → [int, int, int, ...]  (ligne 0)
            [ptr1] → [int, int, int, ...]  (ligne 1)
            [ptr2] → [int, int, int, ...]  (ligne 2)
            ...
```

#### `read_and_fill_rows(int fd, t_map *map)` - Lire et remplir la matrice

**Rôle** : Lire toutes les lignes du fichier et remplir `z_matrix` avec les altitudes.

**Logique** :

```c
int	read_and_fill_rows(int fd, t_map *map)
{
	char	*line;
	int		y;

	y = 0;
	line = get_next_line(fd);
	while (line && y < map->height)
	{
		// 1. Vérifier que la largeur correspond toujours
		if (count_columns(line) != map->width)
			return (free(line), 0);
		
		// 2. Traiter cette ligne : découper, valider et convertir en entiers
		if (!process_line(line, map, y))
			return (free(line), 0);
		
		free(line);
		y++;
		line = get_next_line(fd);
	}
	return (1);
}
```

#### `process_line(char *line, t_map *map, int y)` - Traiter une ligne

**Rôle** : Découper une ligne en nombres, valider chaque nombre, et les stocker dans `z_matrix[y]`.

**Algorithme** :

```c
int	process_line(char *line, t_map *map, int y)
{
	char	**numbers;
	int		x;
	int		i;

	// 1. Découper la ligne en mots (séparés par ' ')
	numbers = ft_split(line, ' ');
	if (!numbers)
		return (0);
	
	x = 0;
	i = 0;
	
	// 2. Pour chaque nombre trouvé
	while (numbers[i])
	{
		// 3. Valider que c'est un entier valide (décimal ou hexadécimal)
		if (!is_valid_integer(numbers[i]))
			return (free_split(numbers), 0);
		
		// 4. Si on n'a pas dépassé la largeur, convertir et stocker
		if (x < map->width)
			map->z_matrix[y][x++] = parse_fdf_value(numbers[i]);
		i++;
	}
	
	// 5. Libérer le tableau de chaînes
	free_split(numbers);
	
	// 6. Remplir les colonnes manquantes avec 0 (si nécessaire)
	while (x < map->width)
		map->z_matrix[y][x++] = 0;
	
	return (1);
}
```

**Exemple** :
- Ligne : `"0 5 0xFF 5 0"`
- Après `ft_split` : `["0", "5", "0xFF", "5", "0"]`
- Après validation et conversion : `z_matrix[y][0] = 0`, `z_matrix[y][1] = 5`, `z_matrix[y][2] = 255`, etc.

#### `is_valid_integer(char *str)` - Valider un nombre

**Rôle** : Vérifier qu'une chaîne représente un nombre valide (décimal ou hexadécimal).

**Logique** :

```c
int	is_valid_integer(char *str)
{
	int		i;
	int		is_hex;

	i = 0;
	// 1. Autoriser un signe +/- au début
	if (str[i] == '+' || str[i] == '-')
		i++;
	
	// 2. Vérifier qu'il y a au moins un caractère après le signe
	if (!str[i])
		return (0);
	
	// 3. Détecter si c'est un nombre hexadécimal (commence par "0x" ou "0X")
	is_hex = 0;
	if ((str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X'))
		&& str[i + 2])
	{
		is_hex = 1;
		i += 2;  // Passer "0x"
	}
	
	// 4. Valider les chiffres (décimaux ou hexadécimaux)
	if (!validate_value_digits(str, &i, is_hex))
		return (0);
	
	// 5. La virgule est acceptée mais la partie après est ignorée
	// (validate_value_digits s'arrête à la virgule, pas de support des couleurs)
	
	return (1);
}
```

**Exemples valides** :
- `"123"` → ✅ (décimal)
- `"-45"` → ✅ (décimal négatif)
- `"0xFF"` → ✅ (hexadécimal)
- `"0xABC"` → ✅ (hexadécimal)
- `"-0xFF"` → ✅ (hexadécimal négatif)

**Exemples invalides** :
- `"12.5"` → ❌ (floats non supportés)
- `"0x"` → ❌ (pas de chiffres après 0x)
- `"abc"` → ❌ (pas de préfixe 0x)

**Exemples avec virgule (acceptés, partie couleur ignorée)** :
- `"123,0xff"` → ✅ (valide comme `123`, partie couleur ignorée)
- `"0xFF,0x00FF00"` → ✅ (valide comme `0xFF` = 255, partie couleur ignorée)

#### `is_hex_digit(char c)` - Vérifier un caractère hexadécimal

**Rôle** : Vérifier qu'un caractère est un chiffre hexadécimal valide.

```c
int	is_hex_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	if (c >= 'a' && c <= 'f')
		return (1);
	if (c >= 'A' && c <= 'F')
		return (1);
	return (0);
}
```

#### `validate_value_digits(char *str, int *i, int is_hex)` - Valider les chiffres

**Rôle** : Valider que tous les caractères suivants sont des chiffres valides (décimaux ou hexadécimaux).

```c
int	validate_value_digits(char *str, int *i, int is_hex)
{
	while (str[*i] && str[*i] != ',' && str[*i] != ' ' 
		&& str[*i] != '\t' && str[*i] != '\n')
	{
		if (is_hex && !is_hex_digit(str[*i]))
			return (0);
		if (!is_hex && (str[*i] < '0' || str[*i] > '9'))
			return (0);
		(*i)++;
	}
	return (1);
}
```

#### `parse_fdf_value(char *str)` - Convertir une valeur FdF en entier

**Rôle** : Extraire la partie numérique (avant virgule si présente) et la convertir en entier.

**Logique** :

```c
int	parse_fdf_value(char *str)
{
	char	*value_str;
	int		i;
	int		result;

	// 1. Compter la longueur de la partie numérique (jusqu'à la virgule ou fin)
	i = 0;
	while (str[i] && str[i] != ',' && str[i] != '\n')
		i++;
	
	// 2. Allouer une chaîne pour extraire cette partie
	value_str = malloc(i + 1);
	if (!value_str)
		return (0);
	
	// 3. Copier la partie numérique
	i = 0;
	while (str[i] && str[i] != ',' && str[i] != '\n')
	{
		value_str[i] = str[i];
		i++;
	}
	value_str[i] = '\0';
	
	// 4. Convertir en entier avec ft_atoi_hex (gère décimal et hexadécimal)
	result = ft_atoi_hex(value_str);
	
	// 5. Libérer la chaîne temporaire
	free(value_str);
	
	return (result);
}
```

**Exemples** :
- `"123"` → `123`
- `"0xFF"` → `255`
- `"0xABC"` → `2748`
- `"-45"` → `-45`
- `"-0xFF"` → `-255`
- `"123,0xff00"` → `123` (extrait la partie avant la virgule)

#### `free_split(char **arr)` - Libérer un tableau de chaînes

**Rôle** : Libérer proprement un tableau de chaînes alloué par `ft_split`.

```c
void	free_split(char **arr)
{
	int		i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);  // Libérer chaque chaîne
		i++;
	}
	free(arr);  // Libérer le tableau
}
```

---

### ÉTAPE 3 : Projection (`projection.c`)

**Objectif** : Transformer les coordonnées carte (x, y) + altitude (z) en coordonnées écran (x_écran, y_écran).

#### `project_point(t_point *pt, int z, t_window *win)` - Projection isométrique

**Rôle** : Appliquer la projection isométrique à un point 3D.

**Formules isométriques** :

```
x_écran = (x_carte - y_carte) * cos(30°) * zoom
y_écran = (x_carte + y_carte) * sin(30°) * zoom - z * zoom * facteur
```

Avec :
- `cos(30°) ≈ 0.866`
- `sin(30°) = 0.5`

**Algorithme** :

```c
void	project_point(t_point *pt, int z, t_window *win)
{
	int	old_x;
	int	old_y;

	// 1. Appliquer le zoom aux coordonnées carte
	old_x = pt->x * win->zoom;  // pt->x est la coordonnée carte (avant projection)
	old_y = pt->y * win->zoom;
	
	// 2. Appliquer les formules isométriques
	//    Vue à 30° : cos(30°) = 0.866, sin(30°) = 0.5
	pt->x = (old_x - old_y) * 0.866;                    // Projette horizontalement
	pt->y = (old_x + old_y) * 0.5 - z * win->zoom * 0.5;  // Projette verticalement + altitude
	
	// 3. Centrer la carte au milieu de l'écran
	pt->x += WIDTH / 2;   // Décaler à droite
	pt->y += HEIGHT / 2;  // Décaler vers le bas
}
```

**Exemple** :
- Point carte : (x=5, y=3, z=10)
- Après zoom (zoom=10) : (50, 30)
- Après projection : x_écran = (50-30) * 0.866 = 17.32, y_écran = (50+30) * 0.5 - 50 = -10
- Après centrage : x_écran = 17.32 + 960 = 977.32, y_écran = -10 + 540 = 530

#### `abs_value(int n)` - Valeur absolue

**Rôle** : Simple helper pour la valeur absolue (utilisé dans Bresenham).

**Logique** :

```c
int	abs_value(int n)
{
	if (n < 0)
		return (-n);  // Si négatif, retourner l'opposé
	return (n);       // Sinon, retourner tel quel
}
```

---

### ÉTAPE 4 : Dessin (`drawing.c` + `draw_line.c`)

**Objectif** : Dessiner la carte en reliant les points avec des lignes blanches.

#### `draw_map(t_window *win)` - Dessiner toute la carte

**Rôle** : Parcourir tous les points de la carte et dessiner les connexions.

**Algorithme** :

```c
void	draw_map(t_window *win)
{
	int		x;
	int		y;
	t_point	p1;  // Point temporaire (variable locale)

	// 1. Parcourir chaque point de la carte
	y = 0;
	while (y < win->map->height)
	{
		x = 0;
		while (x < win->map->width)
		{
			// 2. Initialiser les coordonnées carte du point
			p1.x = x;  // Coordonnée X dans la carte
			p1.y = y;  // Coordonnée Y dans la carte
			
			// 3. Projeter le point en coordonnées écran
			project_point(&p1, win->map->z_matrix[y][x], win);
			
			// 4. Dessiner les connexions depuis ce point
			draw_connections(win, &p1, x, y);
			
			x++;
		}
		y++;
	}
}
```

**Complexité** : O(height × width)

#### `draw_connections(t_window *win, t_point *p1, int x, int y)` - Dessiner les connexions

**Rôle** : Dessiner les lignes reliant un point à ses voisins (droite et bas).

**Logique** :

```c
void	draw_connections(t_window *win, t_point *p1, int x, int y)
{
	t_point	p2;  // Point temporaire pour le voisin

	// Connexion vers la DROITE (si pas en bordure)
	if (x < win->map->width - 1)
	{
		p2.x = x + 1;  // Coordonnée carte du voisin de droite
		p2.y = y;
		project_point(&p2, win->map->z_matrix[y][x + 1], win);  // Projeter le voisin
		draw_line(win, p1, &p2);  // Dessiner la ligne
	}
	
	// Connexion vers le BAS (si pas en bordure)
	if (y < win->map->height - 1)
	{
		p2.x = x;
		p2.y = y + 1;  // Coordonnée carte du voisin en dessous
		project_point(&p2, win->map->z_matrix[y + 1][x], win);  // Projeter le voisin
		draw_line(win, p1, &p2);  // Dessiner la ligne
	}
}
```

**Pourquoi seulement droite et bas ?**

- Chaque point dessine ses connexions vers la droite et le bas
- On évite de dessiner chaque ligne deux fois (haut-gauche serait redondant)

#### `put_pixel(t_window *win, int x, int y, int color)` - Écrire un pixel

**Rôle** : Écrire directement dans le buffer image (beaucoup plus rapide que `mlx_pixel_put`).

**Algorithme** :

```c
void	put_pixel(t_window *win, int x, int y, int color)
{
	int	position;

	// 1. Vérifier que le pixel est dans les limites de l'écran
	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		// 2. Calculer la position en mémoire du pixel
		//    Formule : position = (y * taille_ligne) + (x * octets_par_pixel)
		position = y * win->line_length + x * (win->bits_per_pixel / 8);
		
		// 3. Écrire la couleur (32 bits = 4 octets) à cette position
		*(unsigned int *)(win->img_data + position) = color;
	}
}
```

**Pourquoi cette formule ?**

- **`y * win->line_length`** : saute toutes les lignes au-dessus de y
- **`x * (win->bits_per_pixel / 8)`** : saute les pixels avant x sur la ligne
- **`win->bits_per_pixel / 8`** : convertit les bits en octets (32 bits = 4 octets)

#### `draw_line(t_window *win, t_point *p1, t_point *p2)` - Dessiner une ligne

**Rôle** : Dessiner une ligne entre deux points en choisissant l'algorithme approprié.

**Logique** :

```c
void	draw_line(t_window *win, t_point *p1, t_point *p2)
{
	// 1. Déterminer si la ligne est plus horizontale ou verticale
	if (abs_value(p2->x - p1->x) >= abs_value(p2->y - p1->y))
	{
		// Ligne plus horizontale → utiliser line_horizontal
		if (p1->x > p2->x)
			line_horizontal(win, p2, p1);  // Inverser si nécessaire
		else
			line_horizontal(win, p1, p2);
	}
	else
	{
		// Ligne plus verticale → utiliser line_vertical
		if (p1->y > p2->y)
			line_vertical(win, p2, p1);  // Inverser si nécessaire
		else
			line_vertical(win, p1, p2);
	}
}
```

**Pourquoi deux algorithmes ?**

- **Ligne horizontale** : on incrémente X, on ajuste Y si nécessaire
- **Ligne verticale** : on incrémente Y, on ajuste X si nécessaire
- Plus efficace que de gérer tous les cas dans une seule fonction

#### `line_horizontal(t_window *win, t_point *p1, t_point *p2)` - Algorithme de Bresenham horizontal

**Rôle** : Dessiner une ligne en incrémentant X pixel par pixel.

**Algorithme de Bresenham** :

```c
void	line_horizontal(t_window *win, t_point *p1, t_point *p2)
{
	int	dx;
	int	dy;
	int	err;
	int	dir_y;

	// 1. Calculer les distances
	dx = abs_value(p2->x - p1->x);  // Distance horizontale
	dy = abs_value(p2->y - p1->y);  // Distance verticale
	
	// 2. Déterminer la direction verticale (monter ou descendre)
	dir_y = (p1->y < p2->y);
	if (dir_y == 0)
		dir_y = -1;  // Si égal, dir_y = -1 (descendre)
	
	// 3. Initialiser l'erreur (permet de décider quand monter/descendre)
	err = dx / 2;
	
	// 4. Parcourir la ligne pixel par pixel
	while (p1->x <= p2->x)
	{
		put_pixel(win, p1->x, p1->y, COLOR_WHITE);  // Dessiner le pixel actuel
		err -= dy;  // Diminuer l'erreur
		
		// 5. Si erreur négative, on doit monter/descendre
		if (err < 0)
		{
			p1->y += dir_y;  // Ajuster Y
			err += dx;       // Réinitialiser l'erreur
		}
		
		p1->x++;  // Avancer horizontalement
	}
}
```

#### `line_vertical(t_window *win, t_point *p1, t_point *p2)` - Algorithme de Bresenham vertical

**Rôle** : Dessiner une ligne en incrémentant Y pixel par pixel.

**Algorithme** : Identique à `line_horizontal` mais en inversant X et Y.

---

### ÉTAPE 5 : Affichage (`display.c`)

**Objectif** : Initialiser MinilibX, calculer le zoom, gérer les événements.

#### `init_mlx(t_window *win)` - Initialiser MinilibX

**Rôle** : Créer la connexion X11, la fenêtre, l'image et obtenir le buffer mémoire.

**Logique** :

```c
void	init_mlx(t_window *win)
{
	// 1. Initialiser la connexion avec le serveur X11
	win->mlx = mlx_init();
	if (!win->mlx)
	{
		ft_putstr_fd("Error: MLX init failed\n", 2);
		exit(1);
	}
	
	// 2. Créer la fenêtre graphique
	win->win = mlx_new_window(win->mlx, WIDTH, HEIGHT, TITLE);
	if (!win->win)
	{
		ft_putstr_fd("Error: window creation failed\n", 2);
		exit(1);
	}
	
	// 3. Créer une image (buffer mémoire)
	win->img = mlx_new_image(win->mlx, WIDTH, HEIGHT);
	if (!win->img)
	{
		ft_putstr_fd("Error: image creation failed\n", 2);
		exit(1);
	}
	
	// 4. Obtenir l'adresse mémoire du buffer image
	win->img_data = mlx_get_data_addr(win->img, &win->bits_per_pixel,
			&win->line_length, &win->endian);
}
```

**Pourquoi créer une image séparée ?**

- **Performance** : `mlx_pixel_put` est très lent (appel X11 par pixel)
- **Double buffering** : On dessine tout dans le buffer, puis on affiche d'un coup
- **Pas de scintillement** : L'affichage est instantané

#### `calc_zoom(t_window *win)` - Calculer le zoom automatique

**Rôle** : Calculer un facteur de zoom pour que la carte entière rentre à l'écran.

**Algorithme** :

```c
void	calc_zoom(t_window *win)
{
	int	max_dimension;
	int	zoom_x;
	int	zoom_y;

	// 1. Calculer la dimension maximale de la carte
	max_dimension = win->map->width + win->map->height;
	
	// 2. Calculer le zoom théorique selon X et Y
	zoom_x = WIDTH / max_dimension;   // Combien de pixels par unité carte en X
	zoom_y = HEIGHT / max_dimension;  // Combien de pixels par unité carte en Y
	
	// 3. Prendre le plus petit (garantit que ça rentre dans les deux dimensions)
	if (zoom_x < zoom_y)
		win->zoom = zoom_x;
	else
		win->zoom = zoom_y;
	
	// 4. S'assurer qu'on a au moins un zoom de 1
	if (win->zoom < 1)
		win->zoom = 1;
}
```

**Exemple** :
- Carte : 10×10 (width=10, height=10)
- Écran : 1920×1080
- `max_dimension = 20`
- `zoom_x = 1920 / 20 = 96`
- `zoom_y = 1080 / 20 = 54`
- `zoom = 54` (le plus petit)

#### `key_press(int key, t_window *win)` - Gérer les touches

**Rôle** : Callback appelé quand une touche est pressée.

**Logique** :

```c
int	key_press(int key, t_window *win)
{
	if (key == KEY_ESC)  // Touche Échap (code 65307)
		close_win(win);
	return (0);  // Retourner 0 indique qu'on a géré l'événement
}
```

#### `close_win(t_window *win)` - Fermer proprement

**Rôle** : Libérer toute la mémoire et fermer le programme.

**Logique** :

```c
int	close_win(t_window *win)
{
	// 1. Détruire l'image MLX
	if (win->img)
		mlx_destroy_image(win->mlx, win->img);
	
	// 2. Détruire la fenêtre
	if (win->win)
		mlx_destroy_window(win->mlx, win->win);
	
	// 3. Nettoyer la connexion MLX
	if (win->mlx)
	{
		mlx_destroy_display(win->mlx);
		free(win->mlx);
	}
	
	// 4. Libérer la carte
	if (win->map)
		free_map(win->map);
	
	// 5. Libérer la structure fenêtre
	free(win);
	
	// 6. Quitter le programme
	exit(0);
}
```

#### `free_map(t_map *map)` - Libérer la carte

**Rôle** : Libérer la matrice `z_matrix` et la structure `t_map`.

**Logique** :

```c
void	free_map(t_map *map)
{
	if (!map)
		return ;
	clear_zmatrix(map);  // Libère z_matrix
	free(map);           // Libère la structure
}
```

#### `clear_zmatrix(t_map *map)` - Libérer la matrice

**Rôle** : Libérer toutes les lignes de `z_matrix` puis le tableau de pointeurs.

```c
void	clear_zmatrix(t_map *map)
{
	int	i;

	if (!map || !map->z_matrix)
		return ;
	i = 0;
	while (i < map->height)
	{
		if (map->z_matrix[i])
			free(map->z_matrix[i]);  // Libérer chaque ligne
		i++;
	}
	free(map->z_matrix);  // Libérer le tableau de pointeurs
	map->z_matrix = NULL;
}
```

---

## 🧠 Principes de MinilibX

### Qu'est-ce que MinilibX ?

MinilibX est une **fine couche d'abstraction** au-dessus de X11 (système de fenêtrage Linux). Elle permet de :
- Créer des fenêtres graphiques
- Dessiner des pixels
- Gérer les événements (clavier, souris, fermeture)

### Concepts fondamentaux

#### 1. Connexion MLX (`mlx_init()`)
- Établit une connexion avec le serveur X11
- Retourne un pointeur `void *mlx` (handle de connexion)
- **Obligatoire** : toutes les autres fonctions MLX ont besoin de ce pointeur

#### 2. Fenêtre (`mlx_new_window()`)
- Crée une fenêtre graphique sur l'écran
- Retourne un pointeur `void *win`
- Paramètres : largeur, hauteur, titre

#### 3. Image (`mlx_new_image()`)
- Crée un **buffer mémoire** pour dessiner
- Plus rapide que de dessiner directement dans la fenêtre
- Retourne un pointeur `void *img`

#### 4. Buffer image (`mlx_get_data_addr()`)
- Obtient l'**adresse mémoire brute** du buffer
- On peut écrire directement dedans (très rapide)
- Informations fournies :
  - `img_data` : pointeur vers les pixels
  - `bits_per_pixel` : nombre de bits par pixel (généralement 32)
  - `line_length` : taille d'une ligne en octets
  - `endian` : ordre des octets

#### 5. Affichage (`mlx_put_image_to_window()`)
- Copie le buffer image dans la fenêtre
- À faire **après** avoir dessiné tout ce qu'on veut dans le buffer

#### 6. Hooks (`mlx_hook()`)
- Enregistre des **callbacks** pour les événements
- Types d'événements :
  - `2` : touche pressée (KeyPress)
  - `17` : fermeture fenêtre (DestroyNotify)
- MLX appelle automatiquement la fonction callback

#### 7. Boucle (`mlx_loop()`)
- **Bloque** le programme et attend les événements
- Le programme reste vivant tant que la fenêtre est ouverte
- Nécessaire pour recevoir les événements

---

## 🔄 Flux Complet du Programme

```
1. main()
   │
   ├─> check_extension() → vérifie .fdf
   │
   └─> start_fdf()
       │
       ├─> malloc(map) + malloc(win)
       │
       └─> parse_map()
           │
           ├─> get_map_dimensions()
           │   ├─> ouvre fichier
           │   ├─> lit ligne par ligne
           │   ├─> count_columns() → compte nombres
           │   ├─> check_line_width() → valide forme rectangulaire
           │   └─> ferme fichier
           │
           ├─> allocate_matrices() → alloue z_matrix[height][width]
           │
           └─> read_and_fill_rows()
               ├─> ouvre fichier
               ├─> pour chaque ligne :
               │   ├─> get_next_line()
               │   ├─> process_line()
               │   │   ├─> ft_split() → découpe en nombres
               │   │   ├─> is_valid_integer() → valide format
               │   │   ├─> parse_fdf_value() → convertit avec ft_atoi_hex
               │   │   └─> stocke dans z_matrix
               │   └─> free(line)
               └─> ferme fichier
       
       └─> init_and_run()
           │
           ├─> init_mlx()
           │   ├─> mlx_init() → connexion X11
           │   ├─> mlx_new_window() → fenêtre
           │   ├─> mlx_new_image() → buffer image
           │   └─> mlx_get_data_addr() → adresse mémoire
           │
           ├─> calc_zoom() → calcul zoom automatique
           │
           ├─> draw_map()
           │   └─> pour chaque point (x, y) :
           │       ├─> project_point() → transforme (x,y,z) en (x_écran, y_écran)
           │       └─> draw_connections()
           │           ├─> project_point() pour voisin droite
           │           ├─> draw_line() → trace ligne (Bresenham)
           │           ├─> project_point() pour voisin bas
           │           └─> draw_line() → trace ligne (Bresenham)
           │
           ├─> mlx_put_image_to_window() → affiche le buffer
           │
           ├─> mlx_hook() → enregistre callbacks (clavier, fermeture)
           │
           └─> mlx_loop() → boucle d'événements (programme reste vivant)
```

---

## ✅ Caractéristiques du Projet

✅ Pas de couleurs (mandatory)  
✅ Projection isométrique standard (30°)  
✅ Zoom automatique  
✅ Gestion ESC et croix  
✅ Fenêtre 1920×1080  
✅ Fichiers organisés (8 fichiers .c)  
✅ Norminette OK  
✅ Fonctions exposées (pas de `static` inutiles)  
✅ Protection fichier `.fdf`  
✅ Support nombres décimaux et hexadécimaux  
✅ Validation forme rectangulaire  
✅ Gestion mémoire propre  

---

## 🚀 Utilisation

```bash
# Compiler
make

# Tester une carte
./fdf test_maps/42.fdf
./fdf test_16x16.fdf

# Nettoyer
make fclean
```

---

## 📝 Fonctions Libft Utilisées

| Fonction | Usage |
|----------|-------|
| `ft_split()` | Découpe ligne en nombres (séparés par ' ') |
| `ft_atoi()` | Convertit chaîne "10" → entier 10 (utilisé par ft_atoi_hex pour décimaux) |
| `ft_atoi_hex()` | Convertit chaîne "0xFF" → entier 255 (dans libft) |
| `ft_strlen()` | Longueur chaîne |
| `ft_strncmp()` | Comparaison de chaînes (vérifie extension .fdf) |
| `get_next_line()` | Lit fichier ligne par ligne |
| `ft_putstr_fd()` | Écrit messages d'erreur dans stderr (fd=2) |

---

## 🔢 Format des Nombres Acceptés

### Décimaux
- `123` → 123
- `-45` → -45
- `+10` → 10
- `0` → 0

### Hexadécimaux
- `0xFF` → 255
- `0xff` → 255 (minuscules acceptées)
- `0xABC` → 2748
- `0x10` → 16
- `-0xFF` → -255 (négatif accepté)

### Rejetés
- `12.5` → ❌ (floats non supportés)
- `123,0xff` → ❌ (virgule = format couleur, non supporté)
- `0x` → ❌ (pas de chiffres après 0x)

---

🎓 **Votre projet FdF est maintenant complet et bien compris !**
