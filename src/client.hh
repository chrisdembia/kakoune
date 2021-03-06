#ifndef client_hh_INCLUDED
#define client_hh_INCLUDED

#include "display_buffer.hh"
#include "env_vars.hh"
#include "input_handler.hh"
#include "safe_ptr.hh"
#include "utils.hh"
#include "option_manager.hh"

namespace Kakoune
{

class UserInterface;
class Window;
class String;
struct Key;

enum class EventMode;

class Client : public SafeCountable, public OptionManagerWatcher
{
public:
    Client(std::unique_ptr<UserInterface>&& ui,
           std::unique_ptr<Window>&& window,
           SelectionList selections,
           EnvVarMap env_vars,
           String name);
    ~Client();

    Client(Client&&) = delete;

    // handle all the keys currently available in the user interface
    void handle_available_input(EventMode mode);

    void print_status(DisplayLine status_line);

    void redraw_ifn();

    UserInterface& ui() const { return *m_ui; }
    Window& window() const { return *m_window; }

    void check_if_buffer_needs_reloading();

    Context& context() { return m_input_handler.context(); }
    const Context& context() const { return m_input_handler.context(); }

    InputHandler& input_handler() { return m_input_handler; }
    const InputHandler& input_handler() const { return m_input_handler; }

    void change_buffer(Buffer& buffer);

    StringView get_env_var(const String& name) const;
    StringView get_env_var(StringView name) const;

private:
    void on_option_changed(const Option& option) override;

    void on_buffer_reload_key(Key key);
    void close_buffer_reload_dialog();
    void reload_buffer();

    Optional<Key> get_next_key(EventMode mode);

    DisplayLine generate_mode_line() const;

    std::unique_ptr<UserInterface> m_ui;
    std::unique_ptr<Window> m_window;

    EnvVarMap m_env_vars;

    InputHandler m_input_handler;

    DisplayLine m_status_line;
    DisplayLine m_pending_status_line;
    DisplayLine m_mode_line;

    Vector<Key, MemoryDomain::Client> m_pending_keys;

    bool m_buffer_reload_dialog_opened = false;
};

}

#endif // client_hh_INCLUDED
