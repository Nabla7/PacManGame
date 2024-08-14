// HighScoreManager.hpp
#ifndef HIGH_SCORE_MANAGER_HPP
#define HIGH_SCORE_MANAGER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

namespace Representation {

    struct HighScore {
        std::string name;
        int score;
        int level;

        HighScore() : name(""), score(0), level(0) {} // Add default constructor
        HighScore(const std::string& n, int s, int l) : name(n), score(s), level(l) {}
    };

    class HighScoreManager {
    public:
        HighScoreManager(const std::string& filename) : filename_(filename) {
            loadScores();
        }

        void addScore(const std::string& name, int score, int level) {
            highScores_.emplace_back(name, score, level);
            std::sort(highScores_.begin(), highScores_.end(),
                      [](const HighScore& a, const HighScore& b) { return a.score > b.score; });
            if (highScores_.size() > 10) {
                highScores_.resize(10);
            }
            saveScores();
        }

        const std::vector<HighScore>& getHighScores() const {
            return highScores_;
        }

    private:
        std::string filename_;
        std::vector<HighScore> highScores_;

        void loadScores() {
            std::ifstream file(filename_);
            if (file.is_open()) {
                std::string name;
                int score, level;
                while (file >> name >> score >> level) {
                    highScores_.emplace_back(name, score, level);
                }
                file.close();
            }
        }

        void saveScores() {
            std::ofstream file(filename_);
            if (file.is_open()) {
                for (const auto& score : highScores_) {
                    file << score.name << " " << score.score << " " << score.level << "\n";
                }
                file.close();
            }
        }
    };

} // namespace Representation

#endif // HIGH_SCORE_MANAGER_HPP