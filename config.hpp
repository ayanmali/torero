using float16_t = _Float16;

constexpr int CONTEXT_WINDOW_SIZE = 512; // 512 tokens
constexpr int VOCAB_SIZE = 50297;

constexpr int MAX_QUEUE_SIZE = 8;
constexpr int MAX_REQ_WIDTH = 8;
constexpr int MAX_REQ_HEIGHT = 8;
