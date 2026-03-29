# Snake Game - Web Edition

Professional web-based implementation of the classic Snake game using HTML5, CSS3, and vanilla JavaScript.

## Features

✅ **Fully Functional Gameplay**
- Grid-based movement system
- Collision detection (walls and self)
- Progressive difficulty (speed increases with score)
- Real-time score tracking

✅ **User Interface**
- Responsive modern design
- Beautiful gradient background
- Live statistics display
- Clear on-canvas rendering with grid visualization
- Visual feedback for game states

✅ **Controls**
- Arrow keys or WASD to move
- Space bar to pause/resume
- R key to restart
- GUI buttons for all actions

✅ **Code Quality**
- Object-oriented design (Game, Snake, Food classes)
- No external dependencies
- Clean, readable, well-commented code
- Proper error handling

## How to Play

1. **Open** `index.html` in any modern web browser
2. **Click** "▶ Start Game" or press any arrow key
3. **Control** the snake using arrow keys or WASD
4. **Eat** red food to grow and score points
5. **Avoid** hitting walls and yourself
6. **Pause** with space bar at any time

## Game Mechanics

- **Starting Length**: 3 segments
- **Initial Speed**: 100ms per move
- **Speed Increase**: Every 30 points
- **Scoring**: 10 points per food eaten
- **Difficulty Levels**: 10+ levels (increasing speed)

## Files

- `index.html` - Main HTML structure and styling
- `game.js` - Game logic and engine

## Browser Support

- Chrome/Edge (latest)
- Firefox (latest)
- Safari (latest)
- Any modern browser with HTML5 Canvas support

## Technical Stack

| Component | Technology |
|-----------|-----------|
| Markup | HTML5 |
| Styling | CSS3 (Flexbox, Gradients) |
| Logic | Vanilla JavaScript (ES6+) |
| Graphics | Canvas API |
| Architecture | Object-Oriented Design |

## Project Structure

```
web/
├── index.html      (UI + Styling)
├── game.js         (Game Logic)
└── README.md       (Documentation)
```

## Game Flow

```
IDLE → START → PLAYING → [PAUSED] → GAME_OVER
         ↑                             ↓
         └─────────── RESTART ←────────┘
```

## Development Notes

- Follow pedagogical principle: "Start small and functional, then improve progressively"
- No external libraries required
- Performance optimized for smooth 60 FPS gameplay
- Responsive design works on desktop, tablet, mobile

## Future Enhancements

Potential improvements (maintaining principle of simplicity first):
- Local high score storage
- Mobile touch controls
- Sound effects
- Different game themes
- Obstacles/power-ups

---

**Status**: ✅ Production Ready
**Last Updated**: March 2026
**Author**: Educational Mini-Project
