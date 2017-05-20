
window.DeepRTS = window.DeepRTS || {};


DeepRTS.game = new Phaser.Game(800, 600, Phaser.AUTO, 'test', { preload: preload, create: create, update: update });

DeepRTS.game.state.add('boot', DeepRTS.states.boot);
//DeepRTS.game.state.add('Preload', DeepRTS.states.preload);
//DeepRTS.game.state.add('Game', DeepRTS.states.game);


function preload() {
}

function create() {
    this.state.start('boot');
}

function update() {
}

