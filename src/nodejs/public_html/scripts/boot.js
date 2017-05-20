window.DeepRTS = window.DeepRTS || {};

window.DeepRTS.states.boot = function(){};

//setting game configuration and loading the assets for the loading screen
window.DeepRTS.states.boot.prototype = {
    preload: function () {
        //assets we'll use in the loading screen
        this.load.image('preloadbar', 'assets/images/preloader-bar.png');
    },

    create: function () {
        //loading screen will have a white background
        this.game.stage.backgroundColor = '#000';

        //scaling options
        this.scale.scaleMode = Phaser.ScaleManager.SHOW_ALL;

        //have the game centered horizontally
        this.scale.pageAlignHorizontally = true;
        this.scale.pageAlignVertically = true;

        //physics system
        this.game.physics.startSystem(Phaser.Physics.ARCADE);

       // this.state.start('preload');
    }
}