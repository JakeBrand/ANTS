#!/usr/bin/env sh
./playgame.py --player_seed 42 --end_wait=0.25 --verbose --log_dir game_logs --turns 1000 --map_file maps/maze/maze_04p_01.map "$@"  "./MyBot" "python sample_bots/python/HunterBot.py" "python sample_bots/python/LeftyBot.py" "python sample_bots/python/HunterBot.py"
