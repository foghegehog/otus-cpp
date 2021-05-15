#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

#include "../include/async.h"

using boost::asio::ip::tcp;

class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket, std::unique_ptr<async::Context>&& context)
    : m_socket(std::move(socket)), m_context(std::move(context))
  {
  }

  void start()
  {
    do_read();
  }

private:
  void do_read()
  {
    auto self(shared_from_this());
    m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
          if (!ec)
          {
            std::cout << "receive " << length << "=" << std::string{m_data, length} << std::endl;
          }
        });
  }

  tcp::socket m_socket;
  std::unique_ptr<async::Context> m_context;
  inline static const int max_length = 1024;
  char m_data[max_length];
};

class server
{
public:
  server(boost::asio::io_context& io_context, short port, int bulk_size)
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port)), m_bulk_size(bulk_size)
  {
    do_accept();
  }

private:
  void do_accept()
  {
    m_acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            auto new_session = std::make_shared<session>(
              std::move(socket),
              std::unique_ptr<async::Context>(async::connect(m_bulk_size)));
            new_session->start();
          }

          do_accept();
        });
  }

  tcp::acceptor m_acceptor;
  int m_bulk_size;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: bulk_server <port> <bulk_size>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    short port = std::atoi(argv[1]);
    int bulk_size = std::atoi(argv[2]);
    server server(io_context, port, bulk_size);

    io_context.run();
  }
  catch (const std::exception& ex)
  {
    std::cerr << "Exception: " << ex.what() << "\n";
  }

  return 0;
}