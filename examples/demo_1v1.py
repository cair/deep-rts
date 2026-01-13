#!/usr/bin/env python3
"""
DeepRTS 1v1 Combat Demo

This example shows two agents fighting each other with random actions.
You can replace the random actions with your own AI agent.

Requirements:
    pip install opencv-python numpy

    # Install DeepRTS (from this repo):
    pip install -e .

Usage:
    python examples/demo_1v1.py

Controls:
    Press 'q' to quit
    Press 's' to save a screenshot
"""

from DeepRTS import Engine
import cv2
import numpy as np


def main():
    print("=" * 50)
    print("DeepRTS 1v1 Combat Demo")
    print("=" * 50)
    print()
    print("Controls:")
    print("  q - Quit")
    print("  s - Save screenshot")
    print("  + - Speed up")
    print("  - - Slow down")
    print()

    # Configure the game
    config = Engine.Config.defaults()
    config.set_gui("Blend2DGui")
    config.set_console_caption_enabled(True)

    # Enable all unit types for more interesting battles
    config.set_footman(True)
    config.set_archer(True)
    config.set_barracks(True)
    config.set_farm(True)

    # Give starting resources so players can build
    config.set_start_gold(500)
    config.set_start_lumber(500)
    config.set_start_stone(200)

    # Create game with a medium-sized map
    game = Engine.Game("21x21-2p-ffa-Eflines.json", config)

    # Add two players
    player0 = game.add_player()
    player1 = game.add_player()

    player0.set_name("Player 1 (Red)")
    player1.set_name("Player 2 (Blue)")

    # Start game
    fps = 30
    game.set_max_fps(fps)
    game.start()

    print(f"Game started! Map: 21x21, FPS: {fps}")
    print()

    episode = 0
    screenshot_count = 0

    while True:
        episode += 1
        game.reset()

        print(f"--- Episode {episode} ---")
        step = 0

        while not game.is_terminal():
            step += 1

            # === YOUR AGENT CODE HERE ===
            # Replace these random actions with your AI agent
            action0 = Engine.Random.get(1, 16)
            action1 = Engine.Random.get(1, 16)

            player0.do_action(action0)
            player1.do_action(action1)
            # ============================

            game.update()

            # Render and display
            image = game.render()

            # Add stats overlay
            overlay = image.copy()
            cv2.rectangle(overlay, (5, 5), (250, 90), (0, 0, 0), -1)
            cv2.addWeighted(overlay, 0.5, image, 0.5, 0, image)

            # Player stats
            cv2.putText(image, f"P1: Gold={player0.gold} Units={player0.num_peasant + player0.num_footman + player0.num_archer}",
                       (10, 25), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (100, 100, 255), 1)
            cv2.putText(image, f"P2: Gold={player1.gold} Units={player1.num_peasant + player1.num_footman + player1.num_archer}",
                       (10, 45), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 100, 100), 1)
            cv2.putText(image, f"Step: {step}  Episode: {episode}",
                       (10, 65), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)
            cv2.putText(image, "Press 'q' to quit, 's' to screenshot",
                       (10, 85), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (150, 150, 150), 1)

            cv2.imshow("DeepRTS 1v1 Combat", image)

            # Handle keyboard input
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q'):
                print("\nQuitting...")
                cv2.destroyAllWindows()
                return
            elif key == ord('s'):
                filename = f"screenshot_{screenshot_count}.png"
                cv2.imwrite(filename, image)
                print(f"Saved {filename}")
                screenshot_count += 1
            elif key == ord('+') or key == ord('='):
                fps = min(fps + 10, 120)
                game.set_max_fps(fps)
                print(f"FPS: {fps}")
            elif key == ord('-'):
                fps = max(fps - 10, 10)
                game.set_max_fps(fps)
                print(f"FPS: {fps}")

        # Episode ended - show results
        p0_score = player0.get_score()
        p1_score = player1.get_score()

        if p0_score > p1_score:
            winner = "Player 1"
        elif p1_score > p0_score:
            winner = "Player 2"
        else:
            winner = "Draw"

        print(f"Episode {episode} finished in {step} steps")
        print(f"  Player 1: Score={p0_score:.1f}, Damage={player0.statistic_damage_done}")
        print(f"  Player 2: Score={p1_score:.1f}, Damage={player1.statistic_damage_done}")
        print(f"  Winner: {winner}")
        print()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nInterrupted by user")
    except ImportError as e:
        print(f"Import error: {e}")
        print()
        print("Make sure you have installed the dependencies:")
        print("  pip install opencv-python numpy")
        print("  pip install -e .  # from the deep-rts-playground directory")
