// Mahmoud Elnagar - 26/09/2022

const xmlEntities = new Map([
  ["<", "&lt;"],
  [">", "&gt;"],
  ['"', "&quot;"],
  ["&", "&amp;"],
]);

/**
 * converts the given AST Node into XML String
 * @param {import("../grammar").ASTNode} ast AST Root Node
 * @return {string} XML
 */
export function astToXml(ast) {
  if (ast.terminal) {
    let body = ast.value;
    if (xmlEntities.has(body)) body = xmlEntities.get(body);
    return `<${ast.type}> ${body} </${ast.type}>`;
  } else {
    let body = ast.value.map((sub) => astToXml(sub)).join("\n");
    if (body.length > 0) body += "\n";
    return `<${ast.type}>\n${body}</${ast.type}>`;
  }
}
