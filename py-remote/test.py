from simplemesh import SimpleMeshParser

print map(ord, SimpleMeshParser(14).make_send_data_cmd(4, 'test'))
