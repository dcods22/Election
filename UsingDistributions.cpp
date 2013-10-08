/**
 * UsingDistributions.cpp - Shows how to use C++11 random number capabilities
 *
 * @author Matthew A Johnson
 * @version 1.0
 * @modified 14 Nov 2012
 * @created 13 Nov 2012
 */
#include <iostream>
#include <iomanip>
#include <random>

/** Simulates fair coin flips. */
class Coin
{
public:
  /** Defines the possible states of a two-sided coin. */
  enum State { HEADS, TAILS };

  /** Creates a coin instance with equal probabilities for heads and tails.*/
  Coin () : m_flipper(0.5),
            m_engine(std::random_device()()) // seed the generator
            {}

  /** Flips the coin. */
  State flip ()
  {
    bool result = m_flipper(m_engine);
    return  (result ? HEADS : TAILS);
  }

private:
  /** Probability distribution that generates true or false. */
  std::bernoulli_distribution m_flipper;
  /** Pseudo-random number generator. */
  std::mt19937 m_engine;
};

/** Used to simulate fair dice. Number of sides is a template argument. */
template <unsigned N>
class Die
{
public:
  /** Creates a die instance with equal probability of landing on any side. */
  Die () : m_roller(N,0,1,[](double x){return 1;}),
           m_engine(std::random_device()()) // seed the generator
           {}

  /** Returns the template parameter directly as the number of sides. */
  unsigned sides() const { return N; }

  /** Rolls the die. */
  unsigned roll ()
  {
    unsigned result = m_roller(m_engine);
    return ++result; // add 1 so our die rolls don't start at zero
  }

private:
  /** Probability distribution that generates random integers from 0 to N. */
  std::discrete_distribution<unsigned> m_roller;
  /** Pseudo-random number generator. */
  std::mt19937 m_engine;
};

int main()
{
  // Instantiate a coin and some dice
  Coin c;
  Die<6> d6;
  Die<12> d12;
  Die<20> d20;

  // Now we test them!

  std::cout << "Coin:";
  for ( auto i = 0; i < 25; ++i )
    std::cout << std::setw(3) << c.flip();

  std::cout << "\n" "D6  :";
  for ( auto i = 0; i < 25; ++i )
    std::cout << std::setw(3) << d6.roll();

  std::cout << "\n" "D12 :";
  for ( auto i = 0; i < 25; ++i )
    std::cout << std::setw(3) << d12.roll();

  std::cout << "\n" "D20 :";
  for ( auto i = 0; i < 25; ++i )
    std::cout << std::setw(3) << d20.roll();
  std::cout << std::endl;

  return 0;
}
