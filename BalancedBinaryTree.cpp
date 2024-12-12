#include <iostream>
using namespace std;

struct Node
{
    int data;
    int height;
    int occurrence;
    Node *left;
    Node *right;
};

// Cria um novo nó
Node *createNode(int data, int height)
{
    Node *newNode = new Node();
    if (!newNode)
    {
        std::cerr << "Erro ao alocar memoria!" << endl;
        return nullptr;
    }
    newNode->data = data;
    newNode->occurrence = 1;
    newNode->height = height;
    newNode->left = newNode->right = nullptr;
    return newNode;
}

// Insere o nó na árvore
Node *insertNode(Node *root, int data)
{
    if (root == nullptr)
    {
        root = createNode(data, root->height);
        cout << "Nó criado com sucesso!" << endl;
        return root;
    }

    if (data == root->data)
        root->occurrence += 1;
    else if (data < root->data)
        root->left = insertNode(root->left, data);
    else
        root->right = insertNode(root->right, data);

    return balanceNode(root);
}

// Função para obter a altura de um nó
int height(Node *node)
{
    return node == nullptr ? 0 : node->height;
}

void updateHeight(Node *node)
{
    node->height = std::max(height(node->left), height(node->right)) + 1;
}

int getBalance(Node *node)
{
    return node == nullptr ? 0 : height(node->left) - height(node->right);
}

// Rotação simples à direita
Node *rotateRight(Node *p, Node *r)
{
    p->left = r->left;
    r->left->right = r;
    r->left = nullptr;

    // Atualiza as alturas
    updateHeight(p);
    updateHeight(r);

    // Retorna o novo nó raiz
    return p;
}

// Rotação simples à esquerda
Node *rotateLeft(Node *p, Node *r)
{
    p->right = r->right;
    r->right->left = r;
    r->right = nullptr;

    // Atualiza as alturas
    updateHeight(p);
    updateHeight(r);

    // Retorna o novo nó raiz
    return p;
}

// Rotação dupla à direita (esquerda-direita)
Node *rotateLeftRight(Node *node)
{
    node->left = rotateLeft(node->left, node->left->left);
    return rotateRight(node, node->right);
}

// Rotação dupla à esquerda (direita-esquerda)
Node *rotateRightLeft(Node *node)
{
    node->right = rotateRight(node->right, node->right->right);
    return rotateLeft(node, node->left);
}

// Função para balancear um nó
Node *balanceNode(Node *node)
{
    // Obtém o fator de balanceamento
    int balance = getBalance(node);

    // Caso Esquerda-Esquerda
    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node, node->right);

    // Caso Direita-Direita
    if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node, node->left);

    // Caso Esquerda-Direita
    if (balance > 1 && getBalance(node->left) < 0)
        return rotateLeftRight(node);

    // Caso Direita-Esquerda
    if (balance < -1 && getBalance(node->right) > 0)
        return rotateRightLeft(node);

    // Se não precisar de rotação, retorna o nó
    return node;
}

// Imprime arvore em ordem crescente
void imprimirOrdemCrescente(Node *root)
{
    if (root == nullptr)
        return;

    imprimirOrdemCrescente(root->left);
    std::cout << root->data << ", ";
    imprimirOrdemCrescente(root->right);
}

// Imprime arvore em ordem decrescente
void imprimirOrdemDecrescente(Node *root)
{
    if (root == nullptr)
        return;

    imprimirOrdemDecrescente(root->right);
    std::cout << root->data << " ";
    imprimirOrdemDecrescente(root->left);
}

// Busca com ocorrencias
void buscarComOcorrencias(Node *root, int value)
{
    if (root == nullptr)
        return;

    if (root->data == value)
    {
        std::cout << "O valor " << value << " foi encontrado " << root->occurrence << " vezes na arvore." << std::endl;
        return;
    }
    else if (value < root->data)
        return buscarComOcorrencias(root->left, value);
    else
        return buscarComOcorrencias(root->right, value);
}

// Método auxiliar para encontrar o nó folha que vai subir
Node *encontrarNoFolhaAEsquerda(Node *node)
{
    if (node->right != nullptr)
    {
        return encontrarNoFolhaAEsquerda(node->right);
    }
    else
    {
        return node;
    }
}

// Método auxiliar para encontrar o nó folha que vai subir
Node *encontrarNoFolhaADireita(Node *node)
{
    if (node->left != nullptr)
    {
        return encontrarNoFolhaADireita(node->left);
    }
    else
    {
        return node;
    }
}

Node *removerNode(Node *root, int value)
{
    if (root == nullptr)
        return nullptr;

    if (value < root->data)
        root->left = removerNode(root->left, value);
    else if (value > root->data)
        root->right = removerNode(root->right, value);
    else
    {
        // Caso 1: Nó folha
        if (root->left == nullptr && root->right == nullptr)
        {
            delete root;
        }

        // Caso 2: Nó com apenas um filho (direita ou esquerda)
        else if (root->left == nullptr)
        {
            Node *temp = root->right;
            delete root;
        }
        else if (root->right == nullptr)
        {
            Node *temp = root->left;
            delete root;
        }
        // Caso 3 e 4: Nó com dois filhos
        else
        {
            // Encontra o sucessor nó folha
            Node *successor = nullptr;
            successor = encontrarNoFolhaAEsquerda(root->left);

            if (successor == nullptr)
            {
                successor = encontrarNoFolhaADireita(root->right);
                // Substitui o valor do nó atual pelo valor do nó folha
                root->data = successor->data;
                // Remove o sucessor in-order
                root->right = removerNode(root->right, successor->data);
            }
            else
            {
                // Substitui o valor do nó atual pelo valor do nó folha
                root->data = successor->data;
                // Remove o sucessor in-order
                root->left = removerNode(root->left, successor->data);
            }
        }
    }

    return balanceNode(root);
}

int main()
{
    int menuController = 0;
    int value;
    Node *root = nullptr;

    while (true)
    {
        cout << "Insira o número para o uso da arvore binaria:" << endl
             << endl
             << "1 - Inserir elemento." << endl
             << "2 - Impressao da arvore em ordem crescente de valores." << endl
             << "3 - Impressao da arvore em ordem decrescente de valores." << endl
             << "4 - Buscar e apresentar a ocorrencia de elemento na arvore." << endl
             << "5 - Remover elemento da arvore." << endl
             << "6 - Sair." << endl;
        cin >> menuController;

        if (menuController == 1)
        {
            cout << "Insira o valor númerico do elemento:" << endl;
            cin >> value;
            root = insertNode(root, value);
        }
        else if (menuController == 2)
        {
            cout << "=========================" << endl
                 << "Elementos:" << endl;
            imprimirOrdemCrescente(root);
            cout << endl
                 << "=========================" << endl;
        }
        else if (menuController == 3)
        {
            cout << "=========================" << endl
                 << "Elementos:" << endl;
            imprimirOrdemDecrescente(root);
            cout << endl
                 << "=========================" << endl;
        }
        else if (menuController == 4)
        {
            cout << "Insira o valor que deseja buscar na arvore:" << endl;
            cin >> value;
            buscarComOcorrencias(root, value);
        }
        else if (menuController == 5)
        {
            cout << "Insira o valor que deseja remover da arvore:" << endl;
            cin >> value;
            removerNode(root, value);
        }
        else if (menuController == 6)
        {
            break;
        }
    }

    return 0;
}