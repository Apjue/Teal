# ![Tiled](./tiled.png) → ![Teal](./teal_mini.png)  
Converts Tiled maps to Teal map format. (CLI)

## Tutorial
### 1) Create a Tiled map
* Download and install Tiled from [the official site](http://mapeditor.org), its user manual is available [here](http://doc.mapeditor.org/en/stable/)
* Open Tiled, create a new map (`File > New > New map`, or press `Ctrl+N`)  

![New map](screenshots/tiled_new_map.png)

* At the left, in the properties window (also openable from `Map > Map Properties`), change the `Stagger Axis` to X  

![Stagger Axis, before changement](screenshots/tiled_stagger_axis_before.png)

* This is what you should have now  

![Stagger Axis, after changement](screenshots/tiled_stagger_axis_after.png)

* At the right, in the Tilesets window (also openable from `View > Views and Toolbars > Tilesets`), add a new tileset  

![Add tileset](screenshots/tiled_add_tileset.png)

* Images path:  
`/Teal/wdirs/data/img/game/main/tileset.png`  
`/Teal/wdirs/data/img/game/main/tileset_fight.png`  
* Click browse, select tileset image, check `Embed in map`, and press `OK`  

![Add tileset window](screenshots/tiled_add_tileset_window.png)

* Repeat previous step with every images mentionned before.  

![Add tileset again](screenshots/tiled_add_tileset_again.png)

* After having added every tilesets, let's add the correct layers. **Layers' names must NOT change !**
* First of all, remove the default tile layer `Tile Layer 1`  

![Remove default layer](screenshots/tiled_remove_default_layer.png)

* Then, add these layers:  
`tex`: Default map tileset, used for textures  
`flag`: Flag tileset  
If the flag tileset is left empty, the tile is considered as invisible.  
Other tiles assign the normal/obstacle/red or blue spawn value to the flag

![Add layers](screenshots/tiled_add_layers.png)

* Now, you can use Tiled to make your map ! But beware, layers are associated to tilesets:  

Layer Name | Tileset
---------- | -------
`tex`      | `tileset`
`flag`     | `tileset_fight`

So, before putting a tile on a layer, make sure the correct layer & tileset are selected (else, since only numbers are exported with the .csv format, wrong tiles will be exported)

### 2) Export the tiled map
* `File > Export As...`, select `CSV Files` in the File's Type list
* (Optional) Copy the 2 `.csv` files in Tiled2Teal's executable directory
* Start Tiled2Teal, and follow the instructions.  

![Tiled2Teal](screenshots/tiled2teal.png)

### 3) Try that map with Teal
Just copy the outputted `.lua` file into `/Teal/wdirs/data/script/maps`. It will be automatically parsed.
