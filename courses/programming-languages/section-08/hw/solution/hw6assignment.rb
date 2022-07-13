# Mahmoud Elnagar 04/07/2022

class MyPiece < Piece

  def self.next_piece (board)
    MyPiece.new(All_My_Pieces.sample, board)
  end

  All_My_Pieces = All_Pieces + [
                 rotations([[0, 0], [1, 0], [0, 1], [1, 1], [2, 1]]), # New Shape 1
                 [[[0, 0], [-2, 0], [-1, 0], [1, 0], [2, 0]], # New Shape 2
                 [[0, 0], [0, -2], [0, -1], [0, 1], [0, 2]]],
                 rotations([[0, 0], [0, 1], [1, 1]]) # New Shape 3
                ] 
end

class MyBoard < Board
  def initialize (game)
    super
    @current_block = MyPiece.next_piece(self)
    @cheat = false
  end

  def cheat
    if (!@cheat && @score >= 100)
      @score -= 100
      @cheat = true
    end
  end

  def next_piece
    if @cheat
      @current_block = MyPiece.new([[[0,0]]], self)
      @cheat = false
    else
      @current_block = MyPiece.next_piece(self)
    end
    @current_pos = nil
  end

  def store_current
    locations = @current_block.current_rotation
    displacement = @current_block.position
    (0..(locations.size-1)).each{|index| 
      current = locations[index];
      @grid[current[1]+displacement[1]][current[0]+displacement[0]] = 
      @current_pos[index]
    }
    remove_filled
    @delay = [@delay - 2, 80].max
  end
  
end

class MyTetris < Tetris

  def set_board
    @canvas = TetrisCanvas.new
    @board = MyBoard.new(self)
    @canvas.place(@board.block_size * @board.num_rows + 3,
                  @board.block_size * @board.num_columns + 6, 24, 80)
    @board.draw
  end

  def key_bindings
    super
    @root.bind('u', proc {@board.rotate_clockwise; @board.rotate_clockwise})
    @root.bind('c', proc {@board.cheat}) 
  end
end

