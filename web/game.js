// Game Constants
const GRID_SIZE = 20;
const CANVAS_WIDTH = 400;
const CANVAS_HEIGHT = 400;
const INITIAL_SPEED = 100; // milliseconds

// Game States
const GameState = {
    IDLE: 'idle',
    PLAYING: 'playing',
    PAUSED: 'paused',
    GAME_OVER: 'gameOver'
};

// Direction Enums
const Direction = {
    UP: 'up',
    DOWN: 'down',
    LEFT: 'left',
    RIGHT: 'right'
};

// Game Class
class Snake {
    constructor() {
        this.reset();
    }

    reset() {
        this.body = [
            { x: 10, y: 10 },
            { x: 9, y: 10 },
            { x: 8, y: 10 }
        ];
        this.direction = Direction.RIGHT;
        this.nextDirection = Direction.RIGHT;
    }

    move() {
        // Update direction
        this.direction = this.nextDirection;

        // Calculate new head position
        const head = this.body[0];
        let newHead = { ...head };

        switch (this.direction) {
            case Direction.UP:
                newHead.y--;
                break;
            case Direction.DOWN:
                newHead.y++;
                break;
            case Direction.LEFT:
                newHead.x--;
                break;
            case Direction.RIGHT:
                newHead.x++;
                break;
        }

        // Add new head
        this.body.unshift(newHead);
    }

    grow() {
        // Don't remove tail - snake grows
        return;
    }

    shrink() {
        // Remove tail
        this.body.pop();
    }

    checkCollision() {
        const head = this.body[0];

        // Wall collision
        if (head.x < 0 || head.x >= CANVAS_WIDTH / GRID_SIZE ||
            head.y < 0 || head.y >= CANVAS_HEIGHT / GRID_SIZE) {
            return true;
        }

        // Self collision (skip first 4 segments to allow initial movement)
        for (let i = 4; i < this.body.length; i++) {
            if (head.x === this.body[i].x && head.y === this.body[i].y) {
                return true;
            }
        }

        return false;
    }

    checkFoodCollision(food) {
        const head = this.body[0];
        return head.x === food.x && head.y === food.y;
    }

    getLength() {
        return this.body.length;
    }
}

class Food {
    constructor() {
        this.spawn();
    }

    spawn(snake = null) {
        let validPosition = false;

        while (!validPosition) {
            this.x = Math.floor(Math.random() * (CANVAS_WIDTH / GRID_SIZE));
            this.y = Math.floor(Math.random() * (CANVAS_HEIGHT / GRID_SIZE));

            // Check if food overlaps with snake
            validPosition = true;
            if (snake) {
                for (let segment of snake.body) {
                    if (segment.x === this.x && segment.y === this.y) {
                        validPosition = false;
                        break;
                    }
                }
            }
        }
    }
}

class Game {
    constructor(canvas) {
        this.canvas = canvas;
        this.ctx = canvas.getContext('2d');
        this.state = GameState.IDLE;
        this.snake = new Snake();
        this.food = new Food();
        this.score = 0;
        this.level = 1;
        this.speed = INITIAL_SPEED;
        this.lastUpdateTime = 0;
        this.gameLoopId = null;

        this.setupEventListeners();
        this.draw();
    }

    setupEventListeners() {
        document.addEventListener('keydown', (e) => this.handleKeyPress(e));
    }

    handleKeyPress(e) {
        switch (e.key.toLowerCase()) {
            case 'arrowup':
            case 'w':
                if (this.snake.direction !== Direction.DOWN) {
                    this.snake.nextDirection = Direction.UP;
                }
                e.preventDefault();
                break;
            case 'arrowdown':
            case 's':
                if (this.snake.direction !== Direction.UP) {
                    this.snake.nextDirection = Direction.DOWN;
                }
                e.preventDefault();
                break;
            case 'arrowleft':
            case 'a':
                if (this.snake.direction !== Direction.RIGHT) {
                    this.snake.nextDirection = Direction.LEFT;
                }
                e.preventDefault();
                break;
            case 'arrowright':
            case 'd':
                if (this.snake.direction !== Direction.LEFT) {
                    this.snake.nextDirection = Direction.RIGHT;
                }
                e.preventDefault();
                break;
            case ' ':
                this.togglePause();
                e.preventDefault();
                break;
            case 'r':
                this.restart();
                e.preventDefault();
                break;
        }
    }

    start() {
        if (this.state === GameState.IDLE || this.state === GameState.GAME_OVER) {
            this.reset();
            this.state = GameState.PLAYING;
            this.updateStatus('Playing... Use arrows to move!', 'playing');
            this.gameLoopId = setInterval(() => this.update(), this.speed);
        }
    }

    togglePause() {
        if (this.state === GameState.PLAYING) {
            this.state = GameState.PAUSED;
            clearInterval(this.gameLoopId);
            this.updateStatus('Paused - Press SPACE to resume', 'paused');
        } else if (this.state === GameState.PAUSED) {
            this.state = GameState.PLAYING;
            this.updateStatus('Playing... Use arrows to move!', 'playing');
            this.gameLoopId = setInterval(() => this.update(), this.speed);
        }
    }

    restart() {
        clearInterval(this.gameLoopId);
        this.reset();
        this.state = GameState.IDLE;
        this.updateStatus('');
        this.draw();
    }

    reset() {
        this.snake = new Snake();
        this.food = new Food();
        this.score = 0;
        this.level = 1;
        this.speed = INITIAL_SPEED;
        this.updateUI();
    }

    update() {
        if (this.state !== GameState.PLAYING) return;

        // Move snake
        this.snake.move();

        // Check collision
        if (this.snake.checkCollision()) {
            this.gameOver();
            return;
        }

        // Check food collision
        if (this.snake.checkFoodCollision(this.food)) {
            this.score += 10;
            this.snake.grow();

            // Increase difficulty every 30 points
            if (this.score % 30 === 0) {
                this.level++;
                this.speed = Math.max(50, INITIAL_SPEED - (this.level - 1) * 10);
                clearInterval(this.gameLoopId);
                this.gameLoopId = setInterval(() => this.update(), this.speed);
            }

            this.food.spawn(this.snake);
            this.updateUI();
        } else {
            // Remove tail if no food eaten
            this.snake.shrink();
        }

        this.draw();
    }

    gameOver() {
        this.state = GameState.GAME_OVER;
        clearInterval(this.gameLoopId);
        this.updateStatus(`GAME OVER! Final Score: ${this.score}`, 'game-over');
        this.draw();
    }

    draw() {
        // Clear canvas
        this.ctx.fillStyle = '#000';
        this.ctx.fillRect(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);

        // Draw grid
        this.ctx.strokeStyle = '#1a1a1a';
        this.ctx.lineWidth = 0.5;
        for (let i = 0; i <= CANVAS_WIDTH; i += GRID_SIZE) {
            this.ctx.beginPath();
            this.ctx.moveTo(i, 0);
            this.ctx.lineTo(i, CANVAS_HEIGHT);
            this.ctx.stroke();

            this.ctx.beginPath();
            this.ctx.moveTo(0, i);
            this.ctx.lineTo(CANVAS_WIDTH, i);
            this.ctx.stroke();
        }

        // Draw snake
        this.snake.body.forEach((segment, index) => {
            if (index === 0) {
                // Head - bright green
                this.ctx.fillStyle = '#10b981';
                this.ctx.shadowColor = '#10b981';
                this.ctx.shadowBlur = 10;
            } else {
                // Body - darker green
                this.ctx.fillStyle = '#059669';
                this.ctx.shadowColor = 'rgba(0, 0, 0, 0)';
                this.ctx.shadowBlur = 0;
            }
            this.ctx.fillRect(
                segment.x * GRID_SIZE + 1,
                segment.y * GRID_SIZE + 1,
                GRID_SIZE - 2,
                GRID_SIZE - 2
            );
        });

        // Draw food
        this.ctx.fillStyle = '#ef4444';
        this.ctx.shadowColor = '#ef4444';
        this.ctx.shadowBlur = 10;
        this.ctx.beginPath();
        this.ctx.arc(
            this.food.x * GRID_SIZE + GRID_SIZE / 2,
            this.food.y * GRID_SIZE + GRID_SIZE / 2,
            GRID_SIZE / 2 - 2,
            0,
            Math.PI * 2
        );
        this.ctx.fill();
        this.ctx.shadowBlur = 0;
    }

    updateUI() {
        document.getElementById('score').textContent = this.score;
        document.getElementById('level').textContent = this.level;
        document.getElementById('length').textContent = this.snake.getLength();
    }

    updateStatus(message, className = '') {
        const statusElement = document.getElementById('status');
        statusElement.textContent = message;
        statusElement.className = 'status ' + className;
    }
}

// Initialize Game
let game;

document.addEventListener('DOMContentLoaded', () => {
    const canvas = document.getElementById('gameCanvas');
    game = new Game(canvas);

    // Button event listeners
    document.getElementById('playBtn').addEventListener('click', () => game.start());
    document.getElementById('pauseBtn').addEventListener('click', () => game.togglePause());
    document.getElementById('restartBtn').addEventListener('click', () => game.restart());
});
