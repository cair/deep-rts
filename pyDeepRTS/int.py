def scroll_process(self):
    if self.gstate.SCROLL_UP:
        self.camera_y = min(0, self.camera_y + 20)
    if self.gstate.SCROLL_DOWN:
        self.camera_y = max(self.window_size[1] - self.canvas_size[1], self.camera_y - 20)
    if self.gstate.SCROLL_LEFT:
        self.camera_x = min(0, self.camera_x + 20)
    if self.gstate.SCROLL_RIGHT:
        self.camera_x = max(self.window_size[0] - self.canvas_size[0], self.camera_x - 20)

def left_click(self, x, y):

    x = max(0, min(MapLoader.width - 1, x))
    y = max(0, min(MapLoader.height - 1, y))

    # Propagate first to Units
    unit_id = self.game.data['unit'][x, y]

    if unit_id != 0:
        # Unit selection
        unit = self.game.units[unit_id]
        self.gstate.selected_unit = unit
        self.player.left_click(x, y, unit_id)
        return
    else:
        # Click on tile
        self.gstate.selected_unit = None

        pass

def right_click(self, x, y):

    x = max(0, min(MapLoader.width - 1, x))
    y = max(0, min(MapLoader.height - 1, y))

    # Right click and hav a selected unit, move it!
    if self.gstate.selected_unit and type(self.gstate.selected_unit.state) != State.Dead:
        self.player.right_click(x, y, self.gstate.selected_unit.unit_id)

def process(self):
    for event in pygame.event.get():

        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        elif event.type == pygame.MOUSEBUTTONDOWN and event.button == Input.LEFT:
            x, y = event.pos
            x += self.camera_x * -1
            y += self.camera_y * -1
            tile_x = int(x / Map.TILE_SIZE)
            tile_y = int(y / Map.TILE_SIZE)
            self.left_click(tile_x, tile_y)

        elif event.type == pygame.MOUSEBUTTONDOWN and event.button == Input.RIGHT:
            x, y = event.pos
            x += self.camera_x * -1
            y += self.camera_y * -1
            tile_x = int(x / Map.TILE_SIZE)
            tile_y = int(y / Map.TILE_SIZE)
            self.right_click(tile_x, tile_y)
        elif (event.type == KEYDOWN):
            if (event.key == K_w):
                self.gstate.SCROLL_UP = True
            elif (event.key == K_a):
                self.gstate.SCROLL_LEFT = True
            elif (event.key == K_s):
                self.gstate.SCROLL_DOWN = True
            elif (event.key == K_d):
                self.gstate.SCROLL_RIGHT = True
        elif event.type == KEYUP:
            if event.key == K_w:
                self.gstate.SCROLL_UP = False
            elif event.key == K_a:
                self.gstate.SCROLL_LEFT = False
            elif event.key == K_s:
                self.gstate.SCROLL_DOWN = False
            elif event.key == K_d:
                self.gstate.SCROLL_RIGHT = False

            # Hotkeys for building
            if self.gstate.selected_unit:
                num_options = len(self.gstate.selected_unit.buildable)
                keys = [K_1, K_2, K_3, K_4, K_5, K_6]
                for idx, key in enumerate(keys[:num_options]):
                    if event.key == key:
                        if self.gstate.selected_unit.build(idx):
                            pass #self.gstate.selected_unit = None




    # Processes
    self.scroll_process()
