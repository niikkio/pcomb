source venv/bin/activate

cpplint --exclude=build --linelength=80 --recursive \
  --filter=-legal/copyright,-build/include_subdir,-build/header_guard,-readability/todo \
  .
