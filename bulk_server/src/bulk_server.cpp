#include <cstdlib>
#include <iostream>
#include <future>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

#include "../include/async.h"

using boost::asio::ip::tcp;

class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket, async::Context* context)
    : m_socket(std::move(socket)), m_context(context)
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
            auto task = std::async(std::launch::async, async::receive, m_context, m_data, length);
            do_read();
            task.wait();
          }
          else if ((ec == boost::asio::error::eof) || (ec == boost::asio::error::connection_reset))
          {
            // handle the disconnect.
            async::disconnect(m_context);
          }
        });
  }

  tcp::socket m_socket;
  async::Context* m_context;
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
              async::connect());
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
    short port;
    int bulk_size;

    if (argc != 3)
    {
      port = 9000;
      bulk_size = 3;
    }
    else
    {
      port = std::atoi(argv[1]);
      bulk_size = std::atoi(argv[2]);
    }

    async::Context::set_bulk_size(bulk_size);

    boost::asio::io_context io_context;

    server server(io_context, port, bulk_size);

    io_context.run();
  }
  catch (const std::exception& ex)
  {
    std::cerr << "Exception: " << ex.what() << "\n";
  }

  return 0;
}