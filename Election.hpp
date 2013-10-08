#ifndef VOTER_H_
#define VOTER_H_
#include <bitset>
#include <memory>
#include <random>
#include <ctime>
#include <vector>
#include <iostream>

namespace Elect
{

	using Ballot = std::bitset<3>;
	
	enum Party {REPUBLICAN, DEMOCRAT, INDEPENDANT};

	//Classes
	class Voter
	{
		public:
			
			Voter () : partyAffiliation(INDEPENDANT){}
			
			virtual bool vote( Ballot & ) = 0;
			
			virtual Party affiliation() const = 0;
			
		private:
		
			Party partyAffiliation;
			

	};

	using Electorate = std::vector<std::unique_ptr<Voter>>;
	
	template<Party P>
	class SwingVoter : public Voter
	{
		public:
			
			SwingVoter ( double enthusiasm, std::initializer_list<double> weight )
				: choice(weight), motovated(enthusiasm), rng(std::time(0)) {}
			
			bool vote( Ballot & b )
			{
			
				bool isVoting = motovated(rng);
								
				if( isVoting )
				{
				
					int result = choice(rng);
					b.flip(result);
					
				}
				
				return isVoting;
			
			}
			
			Party affiliation() const
			{
			
				return P;
				
			}
			
		private:

			std::discrete_distribution <unsigned> choice;
			std::bernoulli_distribution motovated;
			std::mt19937 rng;
			Party partyAffiliation;
			
	};

	template<Party P>
	class BaseVoter : public Voter
	{
		public:
		
			BaseVoter ( double enthusiasm ) 
				: motovated(enthusiasm), rng(std::time(0)) {}
			bool vote( Ballot & b )
			{
				
				bool isVoting = motovated(rng);
				
				if( isVoting )
				{
					b.flip(P);
				}
				
				return isVoting;
			
			}
			
			Party affiliation() const
			{
			
				return P;
				
			}
			
		private:
		
			std::bernoulli_distribution motovated;
			std::mt19937 rng;
			Party partyAffiliation;
			
	};
	
	class Election
	{
		public:
		
			Election ( int popnsize, std::initializer_list<double> strength, std::initializer_list<double> undecided, std::initializer_list<double> turnout, std::initializer_list<std::initializer_list<double>> weight)
				: population(popnsize),strengths(strength),undecide(undecided),turnouts(turnout),weights(std::begin(weight), std::end(weight)), dem_undecided(*undecided.begin()), rep_undecided(*(undecided.begin() + 1)), ind_undecided(*(undecided.begin() + 2)) {}				
		
			void simulate()
			{
			
				Electorate voters;
			
				for(auto i = 0; i < population; i++)
				{
				
					auto prty = (Party) undecide(rng);
					
					switch(prty)
					{
					
						case REPUBLICAN:
						{
						
							bool isSwingR = rep_undecided(rng);
							
							if(isSwingR)
							{
							
								voters.push_back(std::unique_ptr<Voter>(new SwingVoter<REPUBLICAN>(REPUBLICAN, {0.70,0.15,0.15} )));
							
							}
							else
							{
							
								voters.push_back(std::unique_ptr<Voter>(new BaseVoter<REPUBLICAN>(REPUBLICAN)));
							
							}
							
							break;
						}	
						
						case DEMOCRAT:
						{
						
							bool isSwingD = dem_undecided(rng);
							
							if(isSwingD)
							{
							
								voters.push_back(std::unique_ptr<Voter>( new SwingVoter<DEMOCRAT>(DEMOCRAT, {0.70,0.15,0.1} )));
							
							}
							else
							{
							
								voters.push_back(std::unique_ptr<Voter>( new BaseVoter<DEMOCRAT>(DEMOCRAT)));
							
							}
							
							break;
						}
						
						case INDEPENDANT:
						{
						
							bool isSwingI = ind_undecided(rng);
							
							if(isSwingI)
							{
							
								voters.push_back(std::unique_ptr<Voter>(new SwingVoter<INDEPENDANT>(INDEPENDANT, {0.35,0.45,0.20} )));
							
							}
							else
							{
							
								voters.push_back(std::unique_ptr<Voter>(new BaseVoter<INDEPENDANT>(INDEPENDANT)));
							
							}
							
							break;
						}	
						default:
							break;
							
					};
					
				}
					
				int numDemVotes = 0;
				int numRepVotes = 0;
				int numIndVotes = 0;

				std::vector<double>::iterator itr;
				
				for(auto itr = voters.begin(); itr != voters.end(); itr++)
				{
				
					Ballot b;				
					
					(*itr)->vote( b ); 
										
					if(b.test(0))
						numRepVotes++;
					else if(b.test(1))
						numDemVotes++;
					else
						numIndVotes++;
					
				}
							
				std::cout << "There was " << numDemVotes << " for the Democrat Candidate." << std::endl;
				std::cout << "There was " << numRepVotes << " for the Republican Candidate."	<< std::endl;
				std::cout << "There was " << numIndVotes << " for the Independant Candidate." << std::endl;
				
				if(numDemVotes > numRepVotes && numDemVotes > numIndVotes)
					std::cout << "Democrats Win!" << std::endl;
				if(numRepVotes > numIndVotes && numRepVotes > numIndVotes)
					std::cout << "Republicans Win!" << std::endl;
				if(numIndVotes > numRepVotes && numIndVotes > numDemVotes)
					std::cout << "Independants Win!" << std::endl;
			}
		
		private:
			
			int population;
			std::vector<double> strengths;
			std::vector<double>	undecide;
			std::vector<double> turnouts;
			std::vector<std::vector<double>> weights;
			//std::bernoulli_distribution undecided;
			std::bernoulli_distribution dem_undecided;
			std::bernoulli_distribution rep_undecided;
			std::bernoulli_distribution ind_undecided;
			std::mt19937 rng;
	
	};
	

	
}
#endif //VOTER_H_