#include <array>
#include <iostream>
#include <cassert>
#include <algorithm>
#include "Random.h"

struct Card {
    enum CardRank {
        rank_ace,
        rank_2,
        rank_3,
        rank_4,
        rank_5,
        rank_6,
        rank_7,
        rank_8,
        rank_9,
        rank_10,
        rank_jack,
        rank_queen,
        rank_king,
        max_rank,
    };

    enum suits {
        clubs,
        diamonds,
        hearts,
        spades,
        max_suits,
    };

    static constexpr std::array<CardRank, max_rank> allRank{
        rank_ace, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8, rank_9, rank_10, rank_jack, rank_queen,
        rank_king
    };

    static constexpr std::array<suits,max_suits> allSuits{clubs,diamonds,hearts,spades};

    CardRank rank{};
    suits suit{};

    friend std::ostream& operator<<(std::ostream& out,const Card& card) {
        static constexpr std::array<std::string_view,max_rank> Ranks{"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
        static constexpr std::array<char,max_suits> Suit{'C','D','H','S'};
        out<<Ranks[card.rank]<<Suit[card.suit];
        return out;
    }

    int getValue() const {
        static constexpr std::array<int,max_rank> RankValues{11,2,3,4,5,6,7,8,9,10,10,10,10};
        return RankValues[rank];
    }
};

class Deck {
private:
    std::array<Card,52> m_cards{};
    std::size_t m_CardIndex{0};
public:
    Deck() {
        std::size_t count{0};
        for (auto temp_suit:Card::allSuits) {
            for (auto temp_rank:Card::allRank) {
                m_cards[count++]=Card{temp_rank,temp_suit};
            }
        }
    }

    Card dealCard() {
        assert(m_CardIndex!=52);
        return m_cards[m_CardIndex++];
    }

    void shuffleCard() {
        std::shuffle(m_cards.begin(),m_cards.end(),Random::mt);
    }
};

struct Player {
    int player_score{0};
};

namespace Setting {
    constexpr int player_bust{21};
    constexpr int dealer_StopDrawing{17};
}

bool DealerDrawing(Player& dealer,Deck& deck);

char player_choice();

bool your_turn(Player& player,Deck& deck);

void BlackJack(Deck& deck,Player& player,Player& dealer);

bool whoWin(Player& player,Player& dealer);


int main() {
    Deck deck{};
    Player player{};
    Player dealer{};
    BlackJack(deck,player,dealer);
    std::cout<<"The dealer is showing: "<<dealer.player_score<<'\n';
    std::cout<<"YOU have score: "<<player.player_score<<'\n';
    if (your_turn(player,deck)) {
        std::cout<<"You got busted\n";
        std::cout<<"You lose\n";
    }else {
        if (DealerDrawing(dealer,deck)) {
            std::cout<<"Dealer got busted\n";
            std::cout<<"You win\n";
        }else {
            if (whoWin(player,dealer)) {
                std::cout<<"You wins\n";
            }else {
                std::cout<<"Dealer wins\n";
            }
        }
    }
    return 0;
}

bool DealerDrawing(Player& dealer,Deck& deck) {
    while (dealer.player_score<Setting::dealer_StopDrawing) {
        Card card{deck.dealCard()};
        dealer.player_score+=card.getValue();
        std::cout<<"The dealer flips a "<<card<<'.'<<"  They now have: "<<dealer.player_score<<'\n';
    }
    if (dealer.player_score>Setting::player_bust) {
        return true;
    }
    return false;
}

char player_choice() {
    while (true) {
        std::cout<<"(h) to hit or (s) to stand: ";
        char choice{};
        std::cin>>choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<<"That's a invalid input\n";
            continue;
        }
        if (choice!='h' && choice!='s') {
            std::cout<<"That's a invalid input\n";
            continue;
        }
        return choice;
    }
}

bool your_turn(Player& player,Deck& deck) {
    char choice{player_choice()};
    while (choice=='h') {
        Card card{deck.dealCard()};
        player.player_score+=card.getValue();
        std::cout<<"You were dealt "<<card<<'.'<<" You now have: "<<player.player_score<<'\n';
        choice={player_choice()};
    }
    if (player.player_score>Setting::player_bust) {
        return true;
    }
    return false;
}

void BlackJack(Deck& deck,Player& player,Player& dealer) {
    deck.shuffleCard();
    dealer.player_score+=deck.dealCard().getValue();
    player.player_score+=deck.dealCard().getValue();
    player.player_score+=deck.dealCard().getValue();
}

bool whoWin(Player& player,Player& dealer) {
    return (player.player_score>dealer.player_score);
}
