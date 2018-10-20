require 'test/unit'
require_relative 'fib-sequence'

class FibonacciSequenceTest < Test::Unit::TestCase
  def setup
    @fib = FibonacciSequence.new
  end
  
  # do testu budou zahrnuty metody s prefixem "test_", tj. napr. nasledujici
  def test_initial_current
    assert_equal(nil, @fib.current)
  end

  def test_initial_current_idx
    assert_equal(nil, @fib.current_idx)
  end

  def test_initial_next
    assert_equal(0, @fib.next)
  end

  def test_current_after_next
    @fib.next
    assert_equal(0, @fib.current)
  end

  def test_current_after_next
    @fib.next
    assert_equal(0, @fib.current_idx)
  end

  def test_reset_current
    @fib.reset
    assert_equal(nil, @fib.current)
  end

  def test_valid_index_10
    assert_equal(55, @fib[10])
  end

  def test_invalid_index
    assert_equal(nil, @fib[-1])
  end

  def test_next_10_times
    10.times { @fib.next }
    assert_equal(55, @fib.next)
  end

  def test_cmp_next_to_index
    idx = 10

    num_idx = @fib[idx]
    @fib.reset

    idx.times { @fib.next }
    num_next = @fib.next

    assert_equal(num_idx, num_next)
  end
end